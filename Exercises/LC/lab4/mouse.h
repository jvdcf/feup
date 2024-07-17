#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include "utils.h"
#include "i8042.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the i8042 mouse controller
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
 * @brief Struct for handling the mouse
 */
typedef struct {
  Interrupt_Info* ih;
  uint8_t status;       // The Status of the controller.
  struct packet* packet; // Information given by the mouse.
  uint8_t byte_index;   // Auxiliary variable to use when reading a packet. 
} MS_State;

/* packet Struct Reference:
   struct packet {
     uint8_t bytes[3]; // Mouse packet raw bytes   
     bool rb;          // Right mouse button pressed
     bool mb;          // Middle mouse button pressed
     bool lb;          // Left mouse button pressed
     int16_t delta_x;  // Mouse x-displacement: rightwards is positive
     int16_t delta_y;  // Mouse y-displacement: upwards is positive
     bool x_ov;        // Mouse x-displacement overflow
     bool y_ov;        // Mouse y-displacement overflow
   }; */

/**
 * @brief Subscribe to mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @param hook_id address of memory to receive the return value of sys_irqsetpolicy()
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_subscribe_int)(uint8_t* bit_no, int* hook_id);

/**
 * @brief Unsubscribe to mouse interrupts
 * 
 * @param handle address of memory with the value returned by sys_irqsetpolicy()
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_unsubscribe_int)(int32_t* handle);

/**
 * @brief Creates a new mouse management state for interrupt handling
 * 
 * @param bit_no Bit number to be set in the mask returned upon an interrupt
 * @return Return a pointer to the new mouse management state
 */
MS_State* (ms_new_ih)(uint8_t bit_no);

/**
 * @brief Destroys a mouse management state
 * 
 * @param state Pointer to the mouse management state to be destroyed
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_free)(MS_State* state);

/**
 * @brief Next interrupt handling state
 * 
 * @param state Pointer to the mouse management state
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_next)(MS_State* state);

/**
 * @brief Sync a byte with the mouse management state
 * 
 * @param state Pointer to the mouse management state
 * @param byte Byte to be added to the state
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_sync)(MS_State* state, uint8_t byte);

/**
 * @brief Parse the 3 mouse bytes into a packet
 * 
 * @param state Pointer to the mouse management state
 */
void (ms_parse)(MS_State* state);

/**
 * @brief Enable data reporting for the mouse.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_enable_dr)();

/**
 * @brief Disable data reporting for the mouse.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (ms_disable_dr)();

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

#endif // _LCOM_MOUSE_H_
