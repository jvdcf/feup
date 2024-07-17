/**
 * @file controller.c
 * @brief Implementation of all the functions executed in every iteration of the game loop.
 * @authors Guilherme Matos, João Ferreira
 */

#include <lcom/lcf.h>
#include "model.h"
#include "view.h"
#include "controller.h"
#include "utils/utils.h"

// Structures ==================================================================

// Cursor
int16_t cursor_x = 0;
int16_t cursor_y = 0;

// Game
GameState gs = Menu;
#define FREQ 30
#define TIMEOUT 60
int TIMEOUTCLOCK = TIMEOUT;
uint8_t my_random = 0;
Game* game = NULL;

// Timer
#define TM_BIT_NO 0
extern int COUNTER;

// Keyboard
#define KB_BIT_NO 1
KB_State *keyboard;

// Mouse
#define MS_BIT_NO 2
#define MS_FQ 40
MS_State *mouse;

// Video
#define VG_VBE_MODE VBE_MODE_I8
VG_State *video;

// Serial
#define SP_BIT_NO 3
#define SP_START_TURN 0x00

// State Machine Functions =====================================================

void no_op() {}

void sm_menu() {
  if (draw_menu(video))
    debug("draw_menu");
}

void sm_select_boats() {
  // Select randomly the location of the 8 boats:
  // 3 boats of size 2 (destroyer)
  // 2 boats of size 3 (submarine)
  // 2 boats of size 4 (battleship)
  // 1 boat of size 5 (carrier)

  game = new_game();
  enum BoatType boats[] = {destroyer, destroyer, destroyer, submarine, submarine, battleship, battleship, carrier};
  for (int i = 0; i < 8; i++) {
    bool added = false;
    while (!added) {
      uint8_t x = rand() % (BOARD_LENGTH - boats[i]);
      uint8_t y = rand() % (BOARD_LENGTH - boats[i]);
      bool horizontal = rand() % 2;
      added = add_boat(game, boats[i], x, y, horizontal);
    }
  }
  debug_game(game);
  gs = WaitingSelect;
}

void sm_waiting_select() {
  if (draw_waiting_start(video)) debug("draw_waiting_start");
}

void sm_my_turn() {
  if (draw_turn(video, game, TIMEOUTCLOCK, true)) debug("draw_my_turn");
  if (draw_mouse(video, cursor_x, cursor_y)) debug("draw_mouse");
}

void sm_waiting_turn() {
  if (draw_turn(video, game, TIMEOUTCLOCK, false)) debug("draw_waiting_turn");
}

void sm_end() {
  if (draw_end(video, is_over(&game->enemy))) debug("draw_end");
}

void sm_serial_waiting_select() {
  if (my_random == 0) {
    my_random = rand();
    sp_push_byte(my_random);
  }
  sp_poll();
  uint8_t received_random;
  if (sp_pop_byte(&received_random)) {
    if (received_random > my_random) gs = MyTurn;
    else if (received_random < my_random) gs = WaitingTurn;
    else {
      my_random = 0;
      gs = WaitingSelect;
    }
  }
}

void sm_serial_my_turn(int x, int y) {
  // Send a packet to the other player with the coordinates
  uint8_t packet = (x << 4) | y;
  sp_push_byte(packet);
  sp_poll();

  // Receive the result of the attack
  uint8_t received_packet;
  sp_busy_wait_for_packet(&received_packet);
  #ifdef DEBUG
  printf("Received packet: %x\n", received_packet);
  #endif

  // Register the attack in the game
  attack(game, x, y, received_packet);

  // Check if the game is over
  if (is_over(&game->enemy)) gs = End;
  else gs = WaitingTurn;
}

void sm_serial_waiting_turn() {
  // Receive the packet with the coordinates of the attack
  uint8_t received_packet;
  sp_poll();
  if (sp_pop_byte(&received_packet)) {
    #ifdef DEBUG
    printf("Received packet: %x\n", received_packet);
    #endif
    uint8_t x = received_packet >> 4;
    uint8_t y = received_packet & 0x0F;

    // Check if the attack hit a boat
    bool has_boat = receive(game, x, y);

    // Send the result of the attack
    sp_push_byte(has_boat);
    sp_poll();

    // Check if the game is over
    if (is_over(&game->player)) gs = End;
    else gs = MyTurn;
  }
}

void sm_timer() {
  COUNTER %= FREQ;
  if (COUNTER == 0) {
    TIMEOUTCLOCK--;
  }
  if (TIMEOUTCLOCK == 0) {
    sm_serial_my_turn(rand() % 16, rand() % 16);
  }
}

void sm_keyboard_menu() {
  bool pressed_enter = ((keyboard->size == 1) && (keyboard->code[0] == 0x9C));
  if (pressed_enter)
    gs = SelectBoats;
}

void sm_keyboard_end() {
  bool pressed_enter = ((keyboard->size == 1) && (keyboard->code[0] == 0x9C));
  if (pressed_enter)
    gs = Menu;
}

void sm_mouse() {
  // Update cursor position
  cursor_x += mouse->packet->delta_x;
  cursor_y -= mouse->packet->delta_y;
  cursor_x = (cursor_x < 0) ? 0 : cursor_x;
  cursor_y = (cursor_y < 0) ? 0 : cursor_y;
  cursor_x = (cursor_x >= video->video_info.XResolution - 12) ? video->video_info.XResolution - 12 : cursor_x;
  cursor_y = (cursor_y >= video->video_info.YResolution - 20) ? video->video_info.YResolution - 20 : cursor_y;

  // If attack
  if (mouse->packet->lb) {
    int x = (cursor_x - BOARD_X) / CELL_SIZE;
    int y = (cursor_y - BOARD_Y_ENEMY) / CELL_SIZE;
    if (x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH) {
      sm_serial_my_turn(x, y);
    }
  }
}

// State Machine Arrays
// ==================================================

void (*const TIMER_FN[])(void) = {
  [Menu] = no_op,
  [SelectBoats] = no_op,
  [WaitingSelect] = no_op,
  [MyTurn] = sm_timer,
  [WaitingTurn] = no_op,
  [End] = no_op,
};

void (*const KEYBOARD_FN[])(void) = {
  [Menu] = sm_keyboard_menu,
  [SelectBoats] = no_op,
  [WaitingSelect] = no_op,
  [MyTurn] = no_op,
  [WaitingTurn] = no_op,
  [End] = sm_keyboard_end};

void (*const MOUSE_FN[])(void) = {
  [Menu] = no_op,
  [SelectBoats] = no_op,
  [WaitingSelect] = no_op,
  [MyTurn] = sm_mouse,
  [WaitingTurn] = no_op,
  [End] = no_op};

void (*const SERIAL_FN[])(void) = {
  [Menu] = no_op,
  [SelectBoats] = no_op,
  [WaitingSelect] = sm_serial_waiting_select,
  [MyTurn] = no_op, // Called by sm_mouse
  [WaitingTurn] = sm_serial_waiting_turn,
  [End] = no_op};

void (*const STATE_FN[])(void) = {
  [Menu] = sm_menu,
  [SelectBoats] = sm_select_boats,
  [WaitingSelect] = sm_waiting_select,
  [MyTurn] = sm_my_turn,
  [WaitingTurn] = sm_waiting_turn,
  [End] = sm_end};

// Methods implementation ======================================================

void initialize_controllers() {
  timer_set_frequency(0, FREQ);
  uint8_t tm_bit_no = TM_BIT_NO;
  timer_subscribe_int(&tm_bit_no);

  keyboard = kb_new_ih(KB_BIT_NO);

  ms_sample_rate(MS_FQ);
  mouse = ms_new_ih(MS_BIT_NO);

  video = vg_new(VG_VBE_MODE);
  initialize_video_memory(video);
  set_graphics_mode(video);

  sp_init(SP_BIT_NO);
}

void free_controllers() {
  timer_unsubscribe_int();
  kb_free(keyboard);
  ms_free(mouse);
  vg_free(video);
  sp_terminate();
}

void game_loop() {
  int r;
  message msg;
  int ipc_status;
  GameState previous = gs;

  while (!pressed_esc(keyboard)) {
    if (previous != gs) {
      #ifdef DEBUG
      printf("[INFO] State changed from %d to %d\n", previous, gs);
      #endif
      previous = gs;
      TIMEOUTCLOCK = TIMEOUT;
    }

    sp_poll();
    SERIAL_FN[previous]();

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("[ERROR] driver_receive failed with %d\n", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & BIT(TM_BIT_NO)) {
            COUNTER++;
            STATE_FN[previous]();
            vg_flush(video);
            TIMER_FN[previous]();
          } 
          if (msg.m_notify.interrupts & BIT(mouse->ih->bit_no)) {
            if (ms_next(mouse)) continue;
            #ifdef DEBUG
            mouse_print_packet(mouse->packet);
            #endif
            MOUSE_FN[previous]();
          } 
          if (msg.m_notify.interrupts & BIT(keyboard->ih->bit_no)) {
            if (kb_next(keyboard)) continue;
            #ifdef DEBUG
            kbd_print_scancode(is_makecode(keyboard), keyboard->size, keyboard->code);
            #endif
            KEYBOARD_FN[previous]();
          } 
          
        default:
          continue;
      }
    }
  }

  printf("[INFO] Exiting game loop by user request. Freeing resources...\n");
  delete_game(game);
}
