#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042 
 * @{
 * 
 * Constants and functions for programming the i8042 KBC. 
 */

#define i_LATENCY    20000
#define i_DELAY(n)   tickdelay(micros_to_ticks(n))

#define IRQ_POL       (IRQ_REENABLE | IRQ_EXCLUSIVE)
#define KBC_STAT_REG   0x64
#define KBC_CMD_REG    0x64
#define KBC_OUT_BUF    0x60
#define KBC_IN_BUF     0x60

#define KB_IRQ_VEC    1
#define KB_READ_COM   0x20
#define KB_WRITE_COM  0x60
#define KB_SELF_TEST  0xAA
#define KB_CHECK_ITF  0xAB
#define KB_OFF_ITF    0xAA

#define MS_IRQ_VEC    12
#define MS_WRITE_BYTE 0xD4
#define MS_ON_DT      0xF4
#define MS_OFF_DT     0xF5
#define MS_ACK        0xFA
#define MS_NACK       0xFE
#define MS_ERROR      0xFC


/**
 * @brief Reads the status of the KBC
 * 
 * @param status address of memory to be initialized with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_read_status)(uint8_t *status);

/**
 * @brief Reads the output buffer of the KBC
 * 
 * @param byte address of memory to be initialized with the output buffer
 * @param status address of memory to be initialized with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_read_outbuf)(uint8_t *byte, uint8_t *status);

/**
 * @brief Writes a command to command register of the KBC
 * 
 * @param cmd command to be written
 * @param status address of memory to be initialized with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_cmd)(uint8_t cmd, uint8_t *status);

/**
 * @brief Writes a command to the input buffer of the KBC
 * 
 * @param byte Data to be written
 * @param status address of memory to be initialized with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_inbuf)(uint8_t byte, uint8_t *status);

#endif /* _LCOM_I8042_H_ */
