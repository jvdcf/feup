/**
 * @file controller.h
 * @brief Definition of the game state machine and the controller in this MVC architecture.
 * @details The controller is responsible for handling the input of the user via the keyboard and mouse, the output via the video card, the timer and the communication with the other player via the serial port. The game state machine is responsible for managing the game state and the transitions between them.
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <lcom/lcf.h>
#include "utils/utils.h"
#include "timer/i8254.h"
#include "timer/timer.h"
#include "kbc/i8042.h"
#include "kbc/keyboard.h"
#include "kbc/mouse.h"
#include "video/vbe.h"
#include "video/video.h"
#include "serial/drv.h"

// Controllers configuration ====================================================

/**
 * @brief Populates the controller variables and initializes every driver. It should be called before the game loop.
 */
void initialize_controllers();

/**
 * @brief Frees the memory allocated for the controllers and closes every driver. It should be called after the game loop.
 */
void free_controllers();

// Game state =================================================================

typedef enum {
  Menu,         /// The main menu of the game. Click ENTER to change to SelectBoats.
  SelectBoats,  /// The board is being populated with the boats. Wait to change to WaitingSelect.
  WaitingSelect,/// Waiting for the other player to finish selecting the boats. Receive the packet to change to MyTurn.
  MyTurn,       /// The player is playing. Select with the mouse the position in the enemy board to attack and change to WaitingTurn. If after 60 seconds the player hasn't selected a position, change to WaitingTurn. If after the attack the player has won, change to End.
  WaitingTurn,  /// Waiting for the other player to attack. Receive the packet to change to MyTurn. If the player has lost, change to End.
  End           /// The game has ended. Click ENTER to change to Menu.
} GameState;

/**
 * @brief Where all the interruptions and the state machine are handled. It closes when the user presses ESC.
*/
void game_loop();

// State machine functions =====================================================

/** @brief No operation function. Do nothing. */
void no_op();
/** @brief In the Menu state, draw the menu. */
void sm_menu();
/** @brief In the SelectBoats state, randomly select the positions for the 8 boats. */
void sm_select_boats();
/** @brief In the WaitingSelect state, draw the waiting menu. */
void sm_waiting_select();
/** @brief In the MyTurn state, draw the boards, the clock and the cursor. */
void sm_my_turn();
/** @brief In the WaitingTurn state, draw only the boards. */
void sm_waiting_turn();
/** @brief In the End state, draw the end menu. */
void sm_end();
/** @brief In the WaitingSelect state, communicate with the other computer to see who plays first.*/
void sm_serial_waiting_select();
/** @brief In the MyTurn state, communicate the attack and receive the result. */
void sm_serial_my_turn(int x, int y);
/** @brief In the WaitingTurn state, receive the attack and communicate the result. */
void sm_serial_waiting_turn();
/** @brief In the MyTurn state, decrement the timer, and if it reaches 0, change to WaitingTurn. */
void sm_timer();
/** @brief In the Menu state, handle the ENTER key in the keyboard. */
void sm_keyboard_menu();
/** @brief In the End state, handle the ENTER key in the keyboard. */
void sm_keyboard_end();
/** @brief In the MyTurn state, handle the mouse packets and call sm_serial_my_turn when the player clicks. */ 
void sm_mouse();


#endif /* _CONTROLLER_H_ */
