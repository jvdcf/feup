#include <lcom/lcf.h>
#include <lcom/lab4.h>
#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "i8042.h"
#include "utils.h"
#include <lcom/timer.h>

#define TM_BIT_NO 0
#define MS_BIT_NO 2

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");
  lcf_log_output("/home/lcom/labs/lab4/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
    uint8_t command_byte = minix_get_dflt_kbc_cmd_byte();
    MS_State* state = ms_new_ih(MS_BIT_NO);

    int r;
    int ipc_status;
    message msg;

    while (cnt > 0) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("[ERROR] driver_receive failed with %d\n", r);
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {

                        if (ms_next(state)) {
                            printf("[ERROR] Failed to handle mouse interrupt\n");
                            continue;
                        }
                        if (state->byte_index == 0) {
                            mouse_print_packet(state->packet);
                            cnt--;
                        }

                    }
                default:
                    continue;
            }
        }
    }

    if (kbc_write_cmd(KBC_CMD_REG, &command_byte)) return 1;
    if (ms_free(state)) return 1;
    return OK;
}

// ===========================================================================

extern int COUNTER; // Used in timer 

int (mouse_test_async)(uint8_t idle_time) {
  int r;
  message msg;
  int ipc_status;

  uint8_t handle_timer = TM_BIT_NO;
  if (timer_subscribe_int(&handle_timer)) return 1;

  uint8_t handle_ms = MS_BIT_NO;
  MS_State* state = ms_new_ih(handle_ms);

  int time = idle_time;

  while (time > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {
            // Mouse interrupt
            if (ms_next(state)) {
                printf("[ERROR] Failed to handle mouse interrupt\n");
                continue;
            }
            if (state->byte_index == 0) {
                mouse_print_packet(state->packet);
                time = idle_time;
                COUNTER = 0;
            }
          }

          if (msg.m_notify.interrupts & BIT(TM_BIT_NO)) {
            // Timer interrupt
            timer_int_handler();
            if (COUNTER % 60 == 0) {
              time--;
            }
          }
        default:
          continue;
      }
    }
  }
  
  if (ms_free(state)) return 1;
  if (timer_unsubscribe_int()) return 1;
  return OK;
}

// ==========================================================================

typedef enum {start, first, middle, second, end} state_machine;

void verify_state(MS_State* ms, state_machine* machine, uint16_t* x_counter, uint8_t x_len, uint8_t tolerance) {
    struct packet* pp = ms->packet;
    bool lb_pressed = pp->lb;
    bool mb_pressed = pp->mb;
    bool rb_pressed = pp->rb;
    float slope = pp->delta_x != 0 ? (float)pp->delta_y / pp->delta_x : 0;
    bool min_slope = abs(slope) > 1;
    bool within_tolerance_x = abs(pp->delta_x) <= tolerance;
    bool within_tolerance_y = abs(pp->delta_y) <= tolerance;

    switch (*machine) {
        case start:
            if (lb_pressed && !mb_pressed && !rb_pressed) {
                *machine = first;
                *x_counter = 0;
            }
            break;

        case first:
            if (lb_pressed && min_slope) {
                *x_counter += abs(pp->delta_x);
            } else if (!lb_pressed) {
                if (*x_counter >= x_len) {
                    *machine = middle;
                    *x_counter = 0;
                } else {
                    *machine = start;
                }
            } else {
                *machine = start;
                *x_counter = 0;
            }
            break;

        case middle:
            if (rb_pressed && !lb_pressed && !mb_pressed) {
                *machine = second;
                *x_counter = 0;
            } else if (!rb_pressed && within_tolerance_x && within_tolerance_y) {
                *machine = middle;
            } else {
                *machine = start;
                *x_counter = 0;
            }
            break;

        case second:
            if (rb_pressed && min_slope) {
                *x_counter += abs(pp->delta_x);
            } else if (!rb_pressed) {
                if (*x_counter >= x_len) {
                    *machine = end;
                } else {
                    *machine = start;
                    *x_counter = 0;
                }
            } else {
                *machine = start;
                *x_counter = 0;
            }
            break;

        default:
            break;
    }
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t command_byte = minix_get_dflt_kbc_cmd_byte();
  MS_State* state = ms_new_ih(MS_BIT_NO);
  state_machine sm = start;
  uint16_t x_counter = 0;

  int r;
  int ipc_status;
  message msg;

  while (true) {
    if (sm == end) break;

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {

            if (ms_next(state)) {
              printf("[ERROR] Failed to handle mouse interrupt\n");
              continue;
            }
            if (state->byte_index == 0) {
              mouse_print_packet(state->packet);
              verify_state(state, &sm, &x_counter, x_len, tolerance);
              printf("State: %d\n", sm);
            }
          }
        default:
          continue;
      }
    }
  }

  if (kbc_write_cmd(KBC_CMD_REG, &command_byte)) return 1;
  if (ms_free(state)) return 1;
  return OK;
}



int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
