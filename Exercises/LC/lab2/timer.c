#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>
// #include <utils.h>

#include "i8254.h"

static int HOOK_ID = 0;
int COUNTER = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2) return 1;
  if (freq == 0) return 1;
  if (TIMER_FREQ / freq > 65535) return 1;
  int errors = 0;

  // 1. Read config
  uint8_t st = 0;
  errors |= timer_get_conf(timer, &st);

  // 2. Change mode to MSB_after_LSB
  uint8_t command = (st | TIMER_LSB_MSB) & 0x3F;
  switch (timer) {
    case 1: command |= TIMER_SEL1; break;
    case 2: command |= TIMER_SEL2; break;
    default: command |= TIMER_SEL0; break;
  }
  errors |= sys_outb(TIMER_CTRL, (uint32_t)command);

  // 3. Send initial value to counter register
  uint16_t initial_value = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  errors |= util_get_LSB(initial_value, &lsb);
  errors |= util_get_MSB(initial_value, &msb);
  int port;
  switch (timer) {
    case 0: port = TIMER_0; break;
    case 1: port = TIMER_1; break;
    case 2: port = TIMER_2; break;
  }
  errors |= sys_outb(port, (uint32_t)lsb);
  errors |= sys_outb(port, (uint32_t)msb);

  return errors;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  HOOK_ID = *bit_no;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &HOOK_ID);
}

int (timer_unsubscribe_int)() { 
  return sys_irqrmpolicy(&HOOK_ID);
}

void (timer_int_handler)() {
  COUNTER++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) return 1;
  if (st == NULL) return 1;
  uint8_t command = TIMER_RB_CMD | (!TIMER_RB_STATUS_ | TIMER_RB_COUNT_) | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL, (uint32_t)command); 
  return util_sys_inb(timer + TIMER_0, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer > 2) return 1;
  union timer_status_field_val status;
  int errors = 0;
  uint8_t tmp = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;

  switch (field) {
    case tsf_all:       // All bits
      status.byte = st;
      break;
    
    case tsf_initial:   // Bits 4, 5
        switch ((st & (BIT(4)|(BIT(5)))) >> 4) {
          case 1:
            status.in_mode = LSB_only;
            break;
          case 2:
            status.in_mode = MSB_only;
            break;
          case 3:
            status.in_mode = MSB_after_LSB;
            break;
          default:
            status.in_mode = INVAL_val;
            errors++;
            break;
        }
      break;
    
    case tsf_mode:    // Bits 1, 2, 3
      status.count_mode = tmp <= 5 ? tmp : tmp - 4;
      break;
    
    case tsf_base:    // bit 0
      status.bcd = (bool)(st & 1);
      break;
  }

  errors += timer_print_config(timer, field, status);
  return errors;
}
