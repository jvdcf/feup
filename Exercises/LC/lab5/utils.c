#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0xFF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = 0;
  int err = sys_inb(port, &val);
  *value = val & 0xFF;
  return err;
}

int(util_sys_outb)(int port, uint8_t value) {
  return sys_outb(port, (uint32_t)value);
}

int(util_max)(unsigned a, unsigned b) {
  return a > b ? a : b;
}

int(util_min)(unsigned a, unsigned b) {
  return a < b ? a : b;
}