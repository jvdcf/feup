#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042 
 * @{
 * 
 * Constants for programming the i8042 KBC. 
 */

#define KB_LATENCY    20000
#define KB_DELAY(n)   tickdelay(micros_to_ticks(n))
#define KB_IRQ_VEC    1
#define KB_IRQ_POL    (IRQ_REENABLE | IRQ_EXCLUSIVE)
#define KB_STAT_REG   0x64
#define KB_OUT_BUF    0x60
#define KB_READ_COM   0x20
#define KB_WRITE_COM  0x60
#define KB_SELF_TEST  0xAA
#define KB_CHECK_ITF  0xAB
#define KB_OFF_ITF    0xAA

#endif /* _LCOM_I8042_H_ */
