#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);

int(util_sys_outb)(int port, uint8_t value);

int(util_max)(unsigned a, unsigned b);

int(util_min)(unsigned a, unsigned b);

