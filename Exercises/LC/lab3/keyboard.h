#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the i8042 keyboard controller
 */

/**
 * @brief Struct for interrupt handling
 */
typedef struct {
  int32_t bit_no;   // Bit Mask for notification.
  int32_t handler;  // The handler for this status.
  uint64_t timeout; // Auxiliary variable for keyboard timeout.
} Interrupt_Info;

/**
 * @brief Struct for handling the keyboard
 */
typedef struct {
  Interrupt_Info* ih;
  uint8_t status;   // The Status of the controller.
  uint8_t code[2];  // Contains keyboard codes.
  size_t   size;    // Number of bytes that have been read. If 0, ignore code.
} KB_State;

// Creates a new keyboard management state for interrupt handling.
KB_State* (kb_new_ih)(uint8_t bit_no);

// Creates a new keyboard management state for polling. 
KB_State* (kb_new_poll)();

// Destroys a keyboard management state. 
int (kb_free)(KB_State *state);

// Next interrupt handling state.
int (kb_next)(KB_State *state);

// Enable keyboard interrupts in KBC
int (kb_enable_int)(void);

// Whether ESC has been pressed and released 
bool (pressed_esc)(KB_State* state);

// Check if scancode is a make or break code
bool (is_makecode)(KB_State* state);

// Subscribe to keyboard interrupts
int (kb_subscribe_int)(uint8_t* bit_no, int* hook_id);

// Removes subscription for keyboard interrupts
int (kb_unsubscribe_int)(int32_t* handle);

// success
#define OK              0
// driver_receive failed
#define ERR_DRIVER_RECV 1
// received a standard message
#define ERR_STD_MSG     2
// didn't find a message for the keyboard 
#define ERR_NOTIF_404   3
// could not read port
#define ERR_STATUS_READ 4
// Parity error
#define ERR_PARITY      5
// Timeout error
#define ERR_TIMEOUT     6
// could not read OUT_BUF 
#define ERR_OUT_BUF     7

#endif // _LCOM_KBC_H_
