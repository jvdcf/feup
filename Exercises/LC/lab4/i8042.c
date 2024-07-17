#include <lcom/lcf.h>
#include "utils.h"
#include "i8042.h"

int (kbc_read_status)(uint8_t *status) {
  if (util_sys_inb(KBC_STAT_REG, status)) {
    printf("[ERROR] failed to read mouse state");
    return 1;
  }
  return 0;
}

int (kbc_read_outbuf)(uint8_t *byte, uint8_t *status) {
  if (kbc_read_status(status)) return 1;
  if (!(*status & BIT(0))) {
    printf("[ERROR] output buffer is empty");
    return 1;
  }
  if (*status & (BIT(6) | BIT(7))) {
    printf("[ERROR] status is invalid (parity or timeout error)");
    return 1;
  }
  if (util_sys_inb(KBC_OUT_BUF, byte)) {
    printf("[ERROR] could not read output buffer");
    return 1;
  }
  return 0;
}

int (kbc_write_cmd)(uint8_t cmd, uint8_t *status) {
  if (kbc_read_status(status)) return 1;
  if (*status & BIT(1)) {
    printf("[ERROR] input buffer is full");
    return 1;
  }
  if (*status & (BIT(6) | BIT(7))) {
    printf("[ERROR] status is invalid (parity or timeout error)");
    return 1;
  }
  if (util_sys_outb(KBC_CMD_REG, cmd)) {
    printf("[ERROR] could not write to command register");
    return 1;
  }
  return 0;
}

int (kbc_write_inbuf)(uint8_t byte, uint8_t *status) {
  if (kbc_read_status(status)) return 1;
  if (*status & BIT(1)) {
    printf("[ERROR] input buffer is full");
    return 1;
  }
  if (*status & (BIT(6) | BIT(7))) {
    printf("[ERROR] status is invalid (parity or timeout error)");
    return 1;
  }
  if (util_sys_outb(KBC_IN_BUF, byte)) {
    printf("[ERROR] could not write to command register");
    return 1;
  }
  return 0;
}
