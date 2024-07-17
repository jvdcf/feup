/**
 * @file keyboard.c
 * @brief Implementation of the driver functions related to the keyboard driver.
 * @authors Guilherme Matos, João Ferreira
 */

#include "keyboard.h"
#include "sys/null.h"
#include "../utils/utils.h"
#include "i8042.h"
#include <lcom/utils.h>
#include <minix/com.h>
#include <minix/driver.h>
#include <minix/endpoint.h>
#include <minix/ipc.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int (kb_unsubscribe_int)(int32_t *handle) {
  return sys_irqrmpolicy(handle);
}

int (kb_subscribe_int)(uint8_t* bit_no, int* hook_id) {
  if (bit_no == NULL || hook_id == NULL) return 1;
  *hook_id = *bit_no;
  return sys_irqsetpolicy(KB_IRQ_VEC, IRQ_POL, hook_id);
}

bool (pressed_esc)(KB_State* state) {
  return ((state->size == 1) && (state->code[0] == 0x81));
}

bool (is_makecode)(KB_State* state) {
  int index = 0;
  if (state->size > 1) index = 1;

  int msb = state->code[index] >> 7;
  return msb == 0;
}

KB_State* (kb_new_ih)(uint8_t bit_no) {
  int hook_id;
  int error = 0;
  if ((error |= kb_subscribe_int(&bit_no, &hook_id)) == 0) {
    KB_State *res = malloc(sizeof(KB_State));
    Interrupt_Info *ih = malloc(sizeof(Interrupt_Info));
    if (res == NULL || ih == NULL) {
      printf("[ERROR] Failed to subscribe to the keyboard notification.");
      exit(error | 101);
    }
    ih->bit_no = bit_no;
    ih->handler = hook_id;
    res->ih = ih;
    res->status = 0;
    res->code[0] = 0;
    res->code[1] = 0;
    res->size = 0;
    return res;
  }
  else {
    printf("[ERROR] Failed to subscribe to the keyboard notification.");
    exit(error);
  }
}

KB_State* (kb_new_poll)(void) {
  int error = 0;
  KB_State *res = malloc(sizeof(KB_State));
  if (res == NULL) {
    printf("[ERROR] Failed to subscribe to the keyboard notification.");
    exit(error | 101);
  }
  res->ih = NULL;
  res->status = 0;
  res->code[0] = 0;
  res->code[1] = 0;
  res->size = 0;
  return res;
}

int (kb_free)(KB_State *state) {
  int error = 0; 
  if (state->ih != NULL) {
    error |= kb_unsubscribe_int(&state->ih->handler);
    free(state->ih);
  }
  free(state);
  return error;
}

int (kb_next)(KB_State *state) {
  if (state->ih != NULL) kbc_ih();

  if (util_sys_inb(KBC_STAT_REG, &state->status) != 0) {
    printf("[ERROR] failed to read keyboard state");
    return ERR_STATUS_READ;
  }
  if ((state->status & BIT(7)) != 0) {
    printf("[ERROR] keyboard reports a parity error");
    return ERR_PARITY;
  }
  if ((state->status & BIT(6)) != 0) {
    printf("[WARN] keyboard timeout");
    return ERR_TIMEOUT;
  }
  if ((state->status & BIT(0)) != 1) {
    return ERR_NOTIF_404;  
  }
  if (util_sys_inb(KBC_OUT_BUF, &state->code[0]) != 0) {
    printf("[ERROR] could not read output buffer");
    return ERR_OUT_BUF;
  }

  state->size = 1;
  if (state->code[0] == 0xE0) {
    if (util_sys_inb(KBC_OUT_BUF, &state->code[1]) != 0) {
      printf("[ERROR] could not read output buffer");
      return ERR_OUT_BUF;
    }
    state->size = 2;
  }

  return OK;
}

void (kbc_ih)(void) {
  // Due to the KB_State structure, it is impossible to implement this function without arguments.
  // This function is only called to pass the labs tests.
  // Use kbc_next() instead.
}

int (kb_enable_int)(void) {
  uint8_t cmd = 0;
  if (util_sys_outb(KBC_STAT_REG, KB_READ_COM) != 0) {
    printf("[ERROR] could not write to status register");
    return ERR_OUT_BUF;
  }
  if (util_sys_inb(KBC_OUT_BUF, &cmd) != 0) {
    printf("[ERROR] could not read output buffer");
    return ERR_OUT_BUF;
  }
  cmd |= BIT(0);
  if (util_sys_outb(KBC_STAT_REG, KB_WRITE_COM) != 0) {
    printf("[ERROR] could not write to status register");
    return ERR_OUT_BUF;
  }
  if (util_sys_outb(KBC_OUT_BUF, cmd) != 0) {
    printf("[ERROR] could not write to output buffer");
    return ERR_OUT_BUF;
  }
  return OK;
}
