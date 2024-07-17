/**
 * @file view.h
 * @brief Definitions of all the drawing functions, i.e., the view of this MVC architecture.
 * @details The view is responsible for the sprites and for drawing every information in the screen using the video card driver, including the menu, the boards, the boats, the cursor and much more.
 * @authors Inês Almeida, Guilherme Matos, João Ferreira, Maria Araújo
*/

#ifndef _VIEW_H_
#define _VIEW_H_

#include <lcom/lcf.h>
#include "model.h"
#include "video/video.h"
#include <lcom/pixmap.h>
#include "pixmap.h"

#define SIDE_LENGTH 330
#define CELL_SIZE (SIDE_LENGTH/BOARD_LENGTH)
#define LIGHT_BLUE 3
#define LIGHTER_BLUE 1
#define BOARD_X 347
#define BOARD_Y_ENEMY 30
#define BOARD_Y_PLAYER 408
#define PLAYER_STRING_X 70
#define PLAYER_STRING_Y 418
#define ENEMY_STRING_X 70
#define ENEMY_STRING_Y 40
#define CLOCK_X 750
#define CLOCK_Y 380

// Menu pages
int draw_menu(VG_State *v);
int draw_waiting_start(VG_State *v);
int draw_turn(VG_State *v, Game *game, uint8_t clock, bool player_turn);
int draw_end(VG_State *v, bool win);
int draw_mouse(VG_State *v, uint16_t x, uint16_t y);

// Auxiliary functions for the game
int draw_board(VG_State *v, Board *b, int start_x, int start_y);
int draw_player_string(VG_State *v);
int draw_enemy_string(VG_State *v);
int draw_clock(VG_State *v, uint8_t clock);
int draw_hit(VG_State *v, uint16_t x, uint16_t y);
int draw_miss(VG_State *v, uint16_t x, uint16_t y);
int draw_boat(VG_State *v, enum BoatType boat_size, uint8_t x, uint8_t y, bool horizontal);


#endif /* _VIEW_H_ */
