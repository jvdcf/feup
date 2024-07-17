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

/**
 * @brief Prepare the reg86 struct for a VBE instruction.
 * @details Call this function, populate the reg86 struct with arguments, if applicable, and call sys_int86(r) to correctly execute the VBE function.
 * 
 * @param r Pointer to the reg86 struct to be prepared.
 * @param function VBE function to be executed
 */
void (prepare_vbe_instruction) (struct reg86* r, uint16_t function);

/**
 * @brief Check if the VBE function was successful.
 * @details Call this function after calling sys_int86(r) to check correctly.
 * 
 * @param r Pointer to the reg86 struct with the result of the VBE function.
 * @return Return 0 upon success and non-zero otherwise
 */
int (check_vbe_return) (struct reg86* r);

#endif /* _LCOM_VBE_H_ */
