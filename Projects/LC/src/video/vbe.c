/**
 * @file vbe.c
 * @brief Implementation of the functions about the VBE standard.
 * @authors Guilherme Matos, Inês Almeida, João Ferreira
 */

#include <lcom/lcf.h>
#include "vbe.h"

void (prepare_vbe_instruction) (struct reg86* r, uint16_t function) {
  memset(r, 0, sizeof(struct reg86));
  r->intno = R_INT;
  r->ah = R_AH;
  r->al = function;
}

int (check_vbe_return) (struct reg86* r) {
  if (r->al != 0x4F) {
    printf("ERROR: VBE function not supported.");
    return 1;
  }
  switch (r->ah) {
    case 0x00:
      return 0;
    case 0x01:
      printf("ERROR: VBE function call failed.");
      return 1;
    case 0x02:
      printf("ERROR: VBE function is not supported in current HW configuration.");
      return 1;
    case 0x03:
      printf("ERROR: VBE function is invalid in current video mode.");
      return 1;
  }
  return 1;
}
