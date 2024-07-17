/**
 * @file view.c
 * @brief Implementation of the functions related to the game view.
 * @authors Inês Almeida, Guilherme Matos, João Ferreira, Maria Araújo
 */

#include "view.h"
#include "model.h"
#include "pixmap.h"
#include <lcom/lcf.h>

int draw_menu(VG_State *v) {

  xpm_image_t game_name;
  if (get_sprite(game_name_xpm, &game_name))
    return 1;
  if (draw_sprite(v, &game_name, 265, 100))
    return 1;

  xpm_image_t M;
  if (get_sprite(M_xpm, &M))
    return 1;
  if (draw_sprite(v, &M, 437, 280))
    return 1;

  xpm_image_t E;
  if (get_sprite(E_xpm, &E))
    return 1;
  if (draw_sprite(v, &E, 477, 280))
    return 1;

  xpm_image_t N;
  if (get_sprite(N_xpm, &N))
    return 1;
  if (draw_sprite(v, &N, 517, 280))
    return 1;

  xpm_image_t U;
  if (get_sprite(U_xpm, &U))
    return 1;
  if (draw_sprite(v, &U, 557, 280))
    return 1;

  xpm_image_t P_play;
  if (get_sprite(P_play_xpm, &P_play))
    return 1;
  if (draw_sprite(v, &P_play, 448, 390))
    return 1;

  xpm_image_t L_play;
  if (get_sprite(L_play_xpm, &L_play))
    return 1;
  if (draw_sprite(v, &L_play, 481, 390))
    return 1;

  xpm_image_t A_play;
  if (get_sprite(A_play_xpm, &A_play))
    return 1;
  if (draw_sprite(v, &A_play, 516, 390))
    return 1;

  xpm_image_t Y_play;
  if (get_sprite(Y_play_xpm, &Y_play))
    return 1;
  if (draw_sprite(v, &Y_play, 549, 390))
    return 1;

  xpm_image_t P_white;
  if (get_sprite(P_white_xpm, &P_white))
    return 1;
  if (draw_sprite(v, &P_white, 255, 600))
    return 1;

  xpm_image_t R_white;
  if (get_sprite(R_white_xpm, &R_white))
    return 1;
  if (draw_sprite(v, &R_white, 287, 600))
    return 1;

  xpm_image_t E_white;
  if (get_sprite(E_white_xpm, &E_white))
    return 1;
  if (draw_sprite(v, &E_white, 319, 600))
    return 1;

  xpm_image_t S_white;
  if (get_sprite(S_white_xpm, &S_white))
    return 1;
  if (draw_sprite(v, &S_white, 351, 600))
    return 1;

  if (draw_sprite(v, &S_white, 383, 600))
    return 1;

  xpm_image_t enter;
  if (get_sprite(enter_xpm, &enter))
    return 1;
  if (draw_sprite(v, &enter, 445, 595))
    return 1;

  xpm_image_t T_white;
  if (get_sprite(T_white_xpm, &T_white))
    return 1;
  if (draw_sprite(v, &T_white, 515, 600))
    return 1;

  xpm_image_t O_white;
  if (get_sprite(O_white_xpm, &O_white))
    return 1;
  if (draw_sprite(v, &O_white, 547, 600))
    return 1;

  if (draw_sprite(v, &S_white, 599, 600))
    return 1;

  if (draw_sprite(v, &T_white, 631, 600))
    return 1;

  xpm_image_t A_white;
  if (get_sprite(A_white_xpm, &A_white))
    return 1;
  if (draw_sprite(v, &A_white, 661, 600))
    return 1;

  if (draw_sprite(v, &R_white, 695, 600))
    return 1;

  if (draw_sprite(v, &T_white, 727, 600))
    return 1;

  return 0;
}

int draw_waiting_start(VG_State *v) {
  xpm_image_t game_name;
  if (get_sprite(game_name_xpm, &game_name))
    return 1;
  if (draw_sprite(v, &game_name, 265, 100))
    return 1;

  xpm_image_t game_start;
  if (get_sprite(game_start_xpm, &game_start))
    return 1;
  if (draw_sprite(v, &game_start, 240, 320))
    return 1;

  xpm_image_t waiting;
  if (get_sprite(waiting_xpm, &waiting))
    return 1;
  if (draw_sprite(v, &waiting, 364, 470))
    return 1;

  return 0;
}

int draw_turn(VG_State *v, Game *game, uint8_t clock, bool player_turn) {
  // Draw boats on player board
  for (int i = 0; i < BOAT_COUNT; i++) {
    Boat boat = game->boats[i];
    if (draw_boat(v, boat.type, boat.x, boat.y, boat.horizontal))
      return 1;
  }

  // Draw boards
  if (draw_board(v, &game->player, BOARD_X, BOARD_Y_PLAYER)) return 1;
  if (draw_board(v, &game->enemy, BOARD_X, BOARD_Y_ENEMY)) return 1;

  // Draw player and enemy strings
  if (draw_player_string(v)) return 1;
  if (draw_enemy_string(v)) return 1;

  // Draw clock
  if (player_turn)
    if (draw_clock(v, clock)) return 1;

  return 0;
}

int draw_end(VG_State *v, bool win) {

  xpm_image_t game_over;
  if (get_sprite(game_over_xpm, &game_over)) return 1;
  if (draw_sprite(v, &game_over, 262, 300)) return 1;

  if (win) {
    xpm_image_t won;
    if (get_sprite(won_xpm, &won)) return 1;
    if (draw_sprite(v, &won, 419, 460)) return 1;
  } else {
    xpm_image_t lost;
    if (get_sprite(lost_xpm, &lost)) return 1;
    if (draw_sprite(v, &lost, 412, 460)) return 1;
  }

  return 0;
}

int draw_mouse(VG_State *v, uint16_t x, uint16_t y) {
  xpm_image_t mouse;
  if (get_sprite(cursor_xpm, &mouse)) return 1;
  if (draw_sprite(v, &mouse, x, y)) return 1;
  return 0;
}

// ==================================================

int draw_board(VG_State *v, Board *b, int start_x, int start_y) {
  xpm_image_t img1, img2;
  if (get_sprite(sprite_board1, &img1))
    return 1;
  if (get_sprite(sprite_board2, &img2))
    return 1;

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      uint16_t x = start_x + i * CELL_SIZE;
      uint16_t y = start_y + j * CELL_SIZE;

      // Decide which image to use based on the cell position
      xpm_image_t *img = ((i + j) % 2 == 0) ? &img1 : &img2;

      // Draw background
      Coord *coord = get_coord(b, i, j);
      if (!coord->is_hit && !coord->has_boat) { // Empty cell
        if (draw_sprite(v, img, x, y))
          return 1;
      }
      else if (coord->is_hit && !coord->has_boat) { // Miss
        if (draw_miss(v, x, y))
          return 1;
      }
      else if (coord->is_hit && coord->has_boat) { // Hit
        if (draw_hit(v, x, y))
          return 1;
      }
    }
  }

  return 0;
}

int draw_player_string(VG_State *v) {
  int x = PLAYER_STRING_X;
  int y = PLAYER_STRING_Y;
  xpm_image_t P_play;
  if (get_sprite(P_play_xpm, &P_play))
    return 1;
  if (draw_sprite(v, &P_play, x, y))
    return 1;

  xpm_image_t L_play;
  if (get_sprite(L_play_xpm, &L_play))
    return 1;
  if (draw_sprite(v, &L_play, x + 32, y))
    return 1;

  xpm_image_t A_play;
  if (get_sprite(A_play_xpm, &A_play))
    return 1;
  if (draw_sprite(v, &A_play, x + 64, y))
    return 1;

  xpm_image_t Y_play;
  if (get_sprite(Y_play_xpm, &Y_play))
    return 1;
  if (draw_sprite(v, &Y_play, x + 96, y))
    return 1;

  xpm_image_t E_play;
  if (get_sprite(E_play_xpm, &E_play))
    return 1;
  if (draw_sprite(v, &E_play, x + 128, y))
    return 1;

  xpm_image_t R_play;
  if (get_sprite(R_play_xpm, &R_play))
    return 1;
  if (draw_sprite(v, &R_play, x + 160, y))
    return 1;

  return 0;
}

int draw_enemy_string(VG_State *v) {
  int x = ENEMY_STRING_X;
  int y = ENEMY_STRING_Y;
  xpm_image_t E_play;
  if (get_sprite(E_play_xpm, &E_play))
    return 1;
  if (draw_sprite(v, &E_play, x, y))
    return 1;

  xpm_image_t N_play;
  if (get_sprite(N_play_xpm, &N_play))
    return 1;
  if (draw_sprite(v, &N_play, x + 32, y))
    return 1;

  if (draw_sprite(v, &E_play, x + 64, y))
    return 1;

  xpm_image_t M_play;
  if (get_sprite(M_play_xpm, &M_play))
    return 1;
  if (draw_sprite(v, &M_play, x + 96, y))
    return 1;

  xpm_image_t Y_play;
  if (get_sprite(Y_play_xpm, &Y_play))
    return 1;
  if (draw_sprite(v, &Y_play, x + 128, y))
    return 1;

  return 0;
}

int draw_clock(VG_State *v, uint8_t clock) {
  xpm_map_t clock_map[10] = {no0_xpm, no1_xpm, no2_xpm, no3_xpm, no4_xpm, no5_xpm, no6_xpm, no7_xpm, no8_xpm, no9_xpm};
  xpm_image_t clock_img;
  uint8_t minutes = clock / 60;
  uint8_t seconds = clock % 60;
  uint8_t m1 = minutes / 10;
  uint8_t m2 = minutes % 10;
  uint8_t s1 = seconds / 10;
  uint8_t s2 = seconds % 10;

  if (get_sprite(clock_map[m1], &clock_img)) return 1;
  if (draw_sprite(v, &clock_img, CLOCK_X, CLOCK_Y)) return 1;

  if (get_sprite(clock_map[m2], &clock_img)) return 1;
  if (draw_sprite(v, &clock_img, CLOCK_X + 30, CLOCK_Y)) return 1;

  if (get_sprite(pontos_xpm, &clock_img)) return 1;
  if (draw_sprite(v, &clock_img, CLOCK_X + 60, CLOCK_Y)) return 1;

  if (get_sprite(clock_map[s1], &clock_img)) return 1;
  if (draw_sprite(v, &clock_img, CLOCK_X + 90, CLOCK_Y)) return 1;

  if (get_sprite(clock_map[s2], &clock_img)) return 1;
  if (draw_sprite(v, &clock_img, CLOCK_X + 120, CLOCK_Y)) return 1;

  return 0;
}

int draw_boat(VG_State *v, enum BoatType boat_size, uint8_t x, uint8_t y, bool horizontal) {
  xpm_map_t boats[8] = {boat2v_xpm, boat2h_xpm, boat3v_xpm, boat3h_xpm, boat4v_xpm, boat4h_xpm, boat5v_xpm, boat5h_xpm};

  xpm_map_t boat_map = boats[(boat_size - 2) * 2 + horizontal];
  xpm_image_t boat_img; 
  if (get_sprite(boat_map, &boat_img)) return 1;
  if (draw_sprite(v, &boat_img, BOARD_X + x * CELL_SIZE, BOARD_Y_PLAYER + y * CELL_SIZE)) return 1;
  
  return 0;
}

int draw_hit(VG_State *v, uint16_t x, uint16_t y) {
  xpm_image_t hit;
  if (get_sprite(hit_xpm, &hit))
    return 1;
  if (draw_sprite(v, &hit, x, y))
    return 1;
  return 0;
}

int draw_miss(VG_State *v, uint16_t x, uint16_t y) {
  xpm_image_t miss;
  if (get_sprite(miss_xpm, &miss))
    return 1;
  if (draw_sprite(v, &miss, x, y))
    return 1;
  return 0;
}
