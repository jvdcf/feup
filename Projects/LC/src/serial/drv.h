/**
 * @file drv.h
 * @brief Declaration of the serial port driver.
 * @author Guilherme Matos
 */

#include <lcom/lcf.h>
#include "../utils/utils.h"

#ifndef SERIAL_H
#  define SERIAL_H

#  define SP1_IRQ 4         // Interrupt line for COM1
#  define SP_UART_B 0x3F8   // Base address for COM1 registers
#  define RCV 0             // Receiver Buffer
#  define TCV 0             // Transceiver Holding Buffer
#  define IER 1             // Interrupt Enable Register
#  define IIR 2             // Interrupt Identification Register
#  define FCR 2             // FIFO Control Register
#  define LCR 3             // Line Control Register
#  define LSR 5             // Line Status Register
#  define DLLSB 0           // Divisor Latch LSB
#  define DLMSB 1           // Divisor Latch MSB

#  define IER_RCVD 1 // Receive Data
#  define IER_TCVE 2 // Transceiver Empty
#  define IER_RCVS 4 // Receiver Line Status

#  define FCR_ENABLE BIT(0) | BIT(7) // ENABLE with 8 long queue
#  define FCR_CLRRCV BIT(1)          // Clear Receiver FIFO
#  define FCR_CLRTCV BIT(2)          // Clear Transceiver FIFO

/**
 * @brief Initializes the serial port driver.
 * @param bit_no The bit number to be used for the interrupt subscription.
 * @return 0 on success, a non-zero value on failure.
 */
int sp_init(int32_t bit_no);

/**
 * @brief Frees the resources used by the serial port driver.
 * @return 0 on success, a non-zero value on failure.
 */
int sp_terminate();

/**
 * @brief Updates the status and contents of the serial port driver. Used with polling.
 * @return 0 on success, the status of the serial port on failure.
 */
int sp_poll();

/**
 * @brief Pushes a byte to the output buffer.
 * @param bite The byte to be pushed.
 * @return 0 on success, a non-zero value on failure.
 */
int sp_push_byte(uint8_t bite);

/**
 * @brief Pops a byte from the input buffer.
 * @param bite The byte to be popped.
 * @return True on success, false otherwise.
 */
bool sp_pop_byte(uint8_t *bite);

/**
 * @brief Waits until a packet is received.
 * @param packet Pointer to the byte where the packet will be stored.
 * @return 0 on success, a non-zero value on failure.
 */
int sp_busy_wait_for_packet(uint8_t *packet);

/**
 * @brief Waits until the output buffer is empty, i.e., all bytes have been sent.
 * @return 0 on success, a non-zero value on failure.
 */
int sp_busy_send_packet();

/**
 * @brief Debug function to print the contents of the serial port driver.
 */
void sp_debug();

/**
 * @brief Prints the configuration of the serial port driver, including the bit-rate, number of stop bits and the parity.
 */
void sp_print_config();

#endif
