#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "../lab3/keyboard.h"
#include <lcom/timer.h>
#include "utils.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  lcf_log_output("/home/lcom/labs/lab5/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

#define KB_BIT_NO 1   // Bit selected for keyboard

// Auxiliary function to sleep the program until the key ESC is pressed (lab3/kbd_test_scan())
void (wait_until_ESC)(KB_State* state) {
  int r;
  message msg;
  int ipc_status;

  while (!pressed_esc(state)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(state->ih->bit_no)) {
            kb_next(state);
          }
        default:
          break;
      }
    }
  }
}

// ==========================================================================

int(video_test_init)(uint16_t mode, uint8_t delay) {
  VG_State* v = vg_new(mode);
  if (set_graphics_mode(v)) return 1;
  sleep(delay);
  if (vg_free(v)) return 1;
  return 0;
}

// ==========================================================================

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  KB_State* kb = kb_new_ih(KB_BIT_NO);
  VG_State* v = vg_new(mode);
  if (initialize_video_memory(v)) return 1;
  if (set_graphics_mode(v)) return 1;

  if (draw_rectangle(v, x, y, width, height, color)) return 1;

  wait_until_ESC(kb);
  if (vg_free(v)) return 1;
  if (kb_free(kb)) return 1;
  return 0;
}

// ==========================================================================

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  KB_State* kb = kb_new_ih(KB_BIT_NO);
  VG_State* v = vg_new(mode);
  if (initialize_video_memory(v)) return 1;
  if (set_graphics_mode(v)) return 1;

  int width = v->video_info.XResolution / no_rectangles;
  int height = v->video_info.YResolution / no_rectangles;

  for (int i = 0; i < no_rectangles; i++) {
    for (int j = 0; j < no_rectangles; j++) {
      uint32_t color;
      switch (v->video_info.MemoryModel) {
        case DIRECT_COLOR:
          color = direct_mode(
            v, 
            red(v, i, step, first), 
            green(v, j, step, first), 
            blue(v, i, j, step, first)
          );
          break;
        default:  // INDEXED_COLOR
          color = indexed_mode(v, i, j, step, first, no_rectangles);
          break;
      }
      if (draw_rectangle(v, i * width, j * height, width, height, color)) return 1;
    }
  }

  wait_until_ESC(kb);
  if (vg_free(v)) return 1;
  if (kb_free(kb)) return 1;
  return 0;
}

// ==========================================================================

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  KB_State* kb = kb_new_ih(KB_BIT_NO);
  VG_State* v = vg_new(VBE_MODE_I8);
  if (initialize_video_memory(v)) return 1;
  if (set_graphics_mode(v)) return 1;

  xpm_image_t img;
  if (get_sprite(xpm, &img)) return 1;
  if (draw_sprite(v, &img, x, y)) return 1;

  wait_until_ESC(kb);
  if (vg_free(v)) return 1;
  if (kb_free(kb)) return 1;
  return 0;
}

// ==========================================================================

#define TM_BIT_NO 0   // Bit selected for timer
extern int COUNTER;   // Counter for timer

bool (wait_timer_and_check_ESC)(KB_State* kb) {
  int ipc_status;
  message msg;
  int r;

  while (!pressed_esc(kb)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(TM_BIT_NO)) {
            return 0;
          }
          else if (msg.m_notify.interrupts & BIT(kb->ih->bit_no)) {
            kb_next(kb);
          }
          break;
        default:
          break;
      }
    }
  }
  return 1;
}

int(close_program)(VG_State* v, KB_State* kb) {
  timer_unsubscribe_int();
  if (vg_free(v)) return 1;
  if (kb_free(kb)) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  KB_State* kb = kb_new_ih(KB_BIT_NO);
  VG_State* v = vg_new(VBE_MODE_I8);
  if (initialize_video_memory(v)) return 1;
  if (set_graphics_mode(v)) return 1;

  xpm_image_t img;
  if (get_sprite(xpm, &img)) return 1;

  if (timer_set_frequency(0, fr_rate)) return 1;
  uint8_t tm_bit_no = TM_BIT_NO;
  if (timer_subscribe_int(&tm_bit_no)) return 1;

  if (speed > 0) {
    uint16_t x = xi;
    uint16_t y = yi;
    while (x < xf || y < yf) {
      uint16_t previous_x = util_max(x - speed, xi);
      uint16_t previous_y = util_max(y - speed, yi);
      draw_rectangle(v, previous_x, previous_y, img.width, img.height, 0x000000); // TODO: clear
      if (draw_sprite(v, &img, x, y)) return 1;
      x = util_min(x + speed, xf);
      y = util_min(y + speed, yf);
      if (wait_timer_and_check_ESC(kb)) return close_program(v, kb);
    }
    draw_rectangle(v, xf - speed, yf - speed, img.width, img.height, 0x000000); // TODO: clear
    if (draw_sprite(v, &img, xf, yf)) return 1;

  } else {
    uint16_t x = xi;
    uint16_t y = yi;
    while (x <= xf || y <= yf) {
      draw_rectangle(v, x-1, y-1, 1, img.height, 0x000000); // TODO: clear
      if (draw_sprite(v, &img, x, y)) return 1;
      for (int i = 0; i < -speed; i++) {
        if (wait_timer_and_check_ESC(kb)) return close_program(v, kb);
      }
      x = util_min(x + 1, xf);
      y = util_min(y + 1, yf);
    }
  }
  
  wait_until_ESC(kb);
  return close_program(v, kb);
}

// ==========================================================================

int(video_test_controller)() {
  /* Not applicable to this year */
  printf("%s(): Residual function\n", __func__);
  return 1;
}
