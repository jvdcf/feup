#include <lcom/lcf.h>
#include "mouse.h"
#include "utils.h"
#include "i8042.h"

int (ms_subscribe_int)(uint8_t* bit_no, int* hook_id) {
  if (bit_no == NULL || hook_id == NULL) return 1;
  *hook_id = *bit_no;
  return sys_irqsetpolicy(MS_IRQ_VEC, IRQ_POL, hook_id);
}

int (ms_unsubscribe_int)(int32_t* handle) {
  return sys_irqrmpolicy(handle);
}

MS_State* (ms_new_ih)(uint8_t bit_no) {
  int hook_id;
  ms_enable_dr();

  if (ms_subscribe_int(&bit_no, &hook_id)) {
    printf("[ERROR] Failed to subscribe mouse interrupts.");
    exit(EXIT_FAILURE);
  }

  MS_State* state =  malloc(sizeof(MS_State));
  Interrupt_Info* ih = malloc(sizeof(Interrupt_Info));
  struct packet* packet = malloc(sizeof(struct packet));
  ih->bit_no = bit_no;
  ih->handler = hook_id;
  ih->timeout = INT_MAX;
  packet->bytes[0] = 0;
  packet->bytes[1] = 0;
  packet->bytes[2] = 0;
  packet->rb = false;
  packet->mb = false;
  packet->lb = false;
  packet->delta_x = 0;
  packet->delta_y = 0;
  packet->x_ov = false;
  packet->y_ov = false;
  state->ih = ih;
  state->packet = packet;
  state->status = 0;
  state->byte_index = 0;
  return state;
}

int (ms_free)(MS_State* state) {
  if (state->ih != NULL) {
    if (ms_unsubscribe_int(&state->ih->handler)) return 1;
    free(state->ih);
  }
  free(state->packet);
  free(state);
  if (ms_disable_dr()) return 1;
  return OK;
}

void (mouse_ih)(void) {
  // Due to the MS_State structure, it is impossible to implement this function without arguments.
  // This function is only called to pass the labs tests.
  // Use ms_next() instead.
}

int (ms_next)(MS_State* state) {
  if (state->ih != NULL) mouse_ih();
  if (kbc_read_outbuf(&state->packet->bytes[state->byte_index], &state->status)) return 1;
  
  state->byte_index++;
  if (state->byte_index == 3) {
    ms_parse(state);
    state->byte_index = 0;
  }
  return OK;
}

int (ms_sync)(MS_State* state, uint8_t byte) {
  // TODO
  return 1;
}

void (ms_parse)(MS_State* state) {
  uint8_t byte0 = state->packet->bytes[0];
  uint8_t byte1 = state->packet->bytes[1];
  uint8_t byte2 = state->packet->bytes[2];

  state->packet->lb = byte0 & BIT(0);
  state->packet->rb = byte0 & BIT(1);
  state->packet->mb = byte0 & BIT(2);
  state->packet->x_ov = byte0 & BIT(6);
  state->packet->y_ov = byte0 & BIT(7);

  bool sign_x = byte0 & BIT(4);
  bool sign_y = byte0 & BIT(5); 

  if (sign_x) state->packet->delta_x = 0xFF00 | byte1;
  else state->packet->delta_x = byte1;

  if (sign_y) state->packet->delta_y = 0xFF00 | byte2;
  else state->packet->delta_y = byte2;
}

int (ms_enable_dr)(void) {
  uint8_t byte = 0;
  uint8_t status = 0;
  if (kbc_write_cmd(MS_WRITE_BYTE, &status)) return 1;
  if (kbc_write_inbuf(MS_ON_DT, &status)) return 1;
  if (kbc_read_outbuf(&byte, &status)) return 1;
  if (byte != MS_ACK) return 1;
  return OK;
}

int (ms_disable_dr)(void) {
  uint8_t byte = 0;
  uint8_t status = 0;
  if (kbc_write_cmd(MS_WRITE_BYTE, &status)) return 1;
  if (kbc_write_inbuf(MS_OFF_DT, &status)) return 1;
  if (kbc_read_outbuf(&byte, &status)) return 1;
  if (byte != MS_ACK) return 1;
  return OK;
}
