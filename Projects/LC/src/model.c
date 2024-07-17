/**
 * @file model.c
 * @brief Implementation of the game logic functions.
 * @authors Guilherme Matos, João Ferreira
 */

#include <lcom/lcf.h>
#include "model.h"

Game* (new_game)() {
  Game* game = (Game*) malloc(sizeof(Game));
  if (game == NULL) {
    printf("ERROR: Failed to allocate memory for game\n");
    return NULL;
  }
  for (int i = 0; i < BOARD_SIZE; i++) {
    game->player.board[i].has_boat = false;
    game->player.board[i].is_hit = false;
    game->enemy.board[i].has_boat = false;
    game->enemy.board[i].is_hit = false;
  }
  game->boat_count = 0;

  return game;
}

void (delete_game)(Game* g) {
  printf("Deleting game\n");
  if (g != NULL) free(g);
}

Coord* (get_coord)(Board* b, uint8_t x, uint8_t y) {
  uint8_t pos = y * BOARD_LENGTH + x;
  return &b->board[pos];
}

bool (add_boat)(Game* g, enum BoatType boat_size, uint8_t x, uint8_t y, bool horizontal) {
  // Check if the boat is out of bounds
  if (x >= BOARD_LENGTH || y >= BOARD_LENGTH) {
    printf("ERROR: Invalid coordinates\n");
    return false;
  }
  if (horizontal && (x + boat_size) > BOARD_LENGTH) {
    printf("ERROR: Boat does not fit in board\n");
    return false;
  }
  if (!horizontal && (y + boat_size) > BOARD_LENGTH) {
    printf("ERROR: Boat does not fit in board\n");
    return false;
  }
  if (g->boat_count >= BOAT_COUNT) {
    printf("ERROR: Maximum number of boats reached\n");
    return false;
  }

  // Check if there is already a boat in the given position
  for (int i = 0; i < (int) boat_size; i++) {
    Coord* coord;
    if (horizontal) coord = get_coord(&g->player, x + i, y);
    else coord = get_coord(&g->player, x, y + i);
    if (coord->has_boat) {
      printf("ERROR: There is already a boat in the given position\n");
      return false;
    }
  }

  // Add boat to the array
  g->boats[g->boat_count].type = boat_size;
  g->boats[g->boat_count].x = x;
  g->boats[g->boat_count].y = y;
  g->boats[g->boat_count].horizontal = horizontal;
  g->boat_count++;

  // Add boat to board
  for (int i = 0; i < (int) boat_size; i++) {
    Coord* coord;
    if (horizontal) coord = get_coord(&g->player, x + i, y);
    else coord = get_coord(&g->player, x, y + i);
    coord->has_boat = true;
  }

  return true;
}

void (attack)(Game* g, uint8_t x, uint8_t y, bool has_boat) {
  Coord* coord = get_coord(&g->enemy, x, y);
  coord->is_hit = true;
  coord->has_boat = has_boat;
}

bool (receive)(Game* g, uint8_t x, uint8_t y) {
  Coord* coord = get_coord(&g->player, x, y);
  coord->is_hit = true;
  return coord->has_boat;
}

bool (is_over)(Board* b) {
  int hit_count = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    Coord* coord = &b->board[i];
    if (coord->is_hit && coord->has_boat) hit_count++;
  }
  return hit_count == SQUARE_COUNT;
}

void debug_game(Game* g) {
  #ifdef DEBUG
  printf("Player board:\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i % BOARD_LENGTH == 0) printf("\n");
    Coord* coord = &g->player.board[i];
    if (coord->has_boat) printf("B ");
    else printf("  ");
  }
  printf("\n\nEnemy board:\n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i % BOARD_LENGTH == 0) printf("\n");
    Coord* coord = &g->enemy.board[i];
    if (coord->has_boat) printf("B ");
    else printf("  ");
  }
  printf("\n\n");

  printf("Boats:\n");
  for (int i = 0; i < g->boat_count; i++) {
    Boat* boat = &g->boats[i];
    printf("Boat %d: %d %d %d\n", i, boat->type, boat->x, boat->y);
  }
  #endif
}
