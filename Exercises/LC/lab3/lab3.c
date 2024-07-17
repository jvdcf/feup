#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "keyboard.h"
#include "utils.h"
#include "i8042.h"
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>

#define TM_BIT_NO 0  // Bit selected for timer
#define KB_BIT_NO 1  // Bit selected for keyboard

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
  lcf_log_output("/home/lcom/labs/lab3/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(kbd_test_scan)() {
  int r;
  message msg;
  int ipc_status;
  int32_t handle = KB_BIT_NO;
  KB_State* state = kb_new_ih(handle);

  while (!pressed_esc(state)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {

            if (kb_next(state) == OK) {
              kbd_print_scancode(is_makecode(state), state->size, state->code);
            }

          }
        default:
        continue;
      }
    }
  }
  if (kb_free(state)) return 1;
  #ifdef LAB3
    kbd_print_no_sysinb(get_count_sys_inb());
  #endif /* ifdef LAB3 */
  return OK;
}

int(kbd_test_poll)() {
  KB_State* state = kb_new_poll();

  while (!pressed_esc(state)) {
    switch (kb_next(state)) {
      case OK:
        kbd_print_scancode(is_makecode(state), state->size, state->code);
      default:
        continue;
    } 
  }

  if (kb_enable_int()) return 1;
  kb_free(state);
  #ifdef LAB3
    kbd_print_no_sysinb(get_count_sys_inb());
  #endif /* ifdef LAB3 */
  return 0;
}

extern int COUNTER; // Used in timer 

int(kbd_test_timed_scan)(uint8_t n) {
  int r;
  message msg;
  int ipc_status;

  uint8_t handle_timer = TM_BIT_NO;
  if (timer_subscribe_int(&handle_timer)) return 1;

  uint8_t handle_kb = KB_BIT_NO;
  KB_State* state = kb_new_ih(handle_kb);

  int time = n;

  while (!pressed_esc(state) && time > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {
            // Keyboard interrupt
            if (kb_next(state) == OK) {
              kbd_print_scancode(is_makecode(state), state->size, state->code);
              time = n;
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
  if (kb_free(state)) return 1;
  if (timer_unsubscribe_int()) return 1;
  return OK;
}
