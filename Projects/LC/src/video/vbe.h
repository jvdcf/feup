/**
 * @file vbe.h
 * @brief Constants and functions for dealing with the VBE standard.
 * @authors Guilherme Matos, Inês Almeida, João Ferreira
 */

#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup vbe vbe
 * @{
 * 
 * Constants and functions for programming in the VBE standard. 
 */

#define R_INT         0x10  // Call via the software interrupt instruction
#define R_AH          0x4F  // Register value for AH when invoking VBE function
#define R_VBE_MODE    0x02  // Register value for AL when setting VBE mode
#define DIRECT_COLOR  0x06
#define INDEXED_COLOR 0x105
#define VBE_MODE_I8   0x105

void (prepare_vbe_instruction) (struct reg86* r, uint16_t function);

int (check_vbe_return) (struct reg86* r);

#endif /* _LCOM_VBE_H_ */
