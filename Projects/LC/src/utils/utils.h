/**
 * @file utils.h
 * @brief Declarations of utility functions.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Struct for interrupt handling
 */
typedef struct {
  int32_t bit_no;   // Bit Mask for notification.
  int32_t handler;  // The handler for this status.
  uint64_t timeout; // Auxiliary variable for keyboard timeout.
} Interrupt_Info;

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);

int(util_sys_outb)(int port, uint8_t value);

void(debug)(char *msg);

#endif /* _UTILS_H_ */
