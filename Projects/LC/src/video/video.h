/**
 * @file video.h
 * @brief Declarations of functions related to the graphics card driver.
 * @authors Guilherme Matos, Inês Almeida, João Ferreira
 */

#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <lcom/lcf.h>
#include "vbe.h"

/** @defgroup video video
 * @{
 *
 * Functions for using the video card in graphics mode.
 */

/**
 * @brief Struct for handling VBE operations and video card
 */
typedef struct {
  uint16_t video_mode;          // Video VBE mode to be set
  struct reg86 r;               // Registers to perform assembly actions
  vbe_mode_info_t video_info;   // Information about VBE mode: screen dimensions, color depth, ...
  uint8_t* base_video_address;  // Start of Video RAM in process's address space
  uint8_t* delta_buffer;        // Auxiliary buffer for delta buffering
} VG_State;

VG_State* (vg_new) (uint16_t mode);

int (vg_free) (VG_State* v);

int (set_graphics_mode) (VG_State* v);

int (initialize_video_memory) (VG_State* v);

int (draw_rectangle) (VG_State* v, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (draw_hline) (VG_State* v, uint16_t x, uint16_t y, uint16_t length, uint32_t color);

int (draw_pixel) (VG_State* v, uint16_t x, uint16_t y, uint32_t color);

uint32_t (direct_mode) (VG_State* v, uint32_t red, uint32_t green, uint32_t blue);

uint32_t (indexed_mode) (VG_State* v, uint16_t column, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Adjust the red component based on the position and step.
*/
uint32_t (red) (VG_State* v, unsigned column, uint8_t step, uint32_t first);

/**
 * @brief Adjust the green component based on the position and step.
*/
uint32_t (green) (VG_State* v, unsigned row, uint8_t step, uint32_t first);

/**
 * @brief Adjust the blue component by combining row and column adjustments.
*/
uint32_t (blue) (VG_State* v, unsigned column, unsigned row, uint8_t step, uint32_t first);

int (get_sprite) (xpm_map_t xpm, xpm_image_t* img);

int (draw_sprite) (VG_State* v, xpm_image_t* img, uint16_t x, uint16_t y);

void (vg_flush) (VG_State* v);


#endif /* _LCOM_VIDEO_H_ */
