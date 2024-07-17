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
} VG_State;

/**
 * @brief Creates a new VGE video management state.
 * 
 * @param mode Video VBE mode to be set
 * @return Return a pointer to the new VG_State
*/
VG_State* (vg_new) (uint16_t mode);

/**
 * @brief Destroys a VGE video management state and exits the video mode.
 * 
 * @param v Pointer to the VG_State to be destroyed
 * @return Return 0 upon success and non-zero otherwise
*/
int (vg_free) (VG_State* v);

/**
 * @brief Set the graphics mode to the one specified in the VG_State.
 * 
 * @param v Pointer to the VG_State
 * @return Return 0 upon success and non-zero otherwise
*/
int (set_graphics_mode) (VG_State* v);

/**
 * @brief Map the physical address to this process and initialize the video memory for the VG_State.
 * 
 * @param v Pointer to the VG_State
 * @return Return 0 upon success and non-zero otherwise
*/
int (initialize_video_memory) (VG_State* v);

/**
 * @brief Draw a rectangle on the screen.
 * 
 * @param v Pointer to the VG_State
 * @param x X coordinate of the top-left corner of the rectangle
 * @param y Y coordinate of the top-left corner of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle in hexadecimal format
 * @return Return 0 upon success and non-zero otherwise
*/
int (draw_rectangle) (VG_State* v, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draw a horizontal line on the screen.
 * 
 * @param v Pointer to the VG_State
 * @param x X coordinate of the top-left corner of the line
 * @param y Y coordinate of the top-left corner of the line
 * @param length Length of the line
 * @param color Color of the line in hexadecimal format
 * @return Return 0 upon success and non-zero otherwise
*/
int (draw_hline) (VG_State* v, uint16_t x, uint16_t y, uint16_t length, uint32_t color);

/**
 * @brief Change the color of a pixel on the screen.
 * 
 * @param v Pointer to the VG_State
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel in hexadecimal format
 * @return Return 0 upon success and non-zero otherwise
*/
int (draw_pixel) (VG_State* v, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Normalize a color to the direct mode.
 * 
 * @param v Pointer to the VG_State
 * @param red Red component of the color
 * @param green Green component of the color
 * @param blue Blue component of the color
 * @return Return the color in hexadecimal format
 */ 
uint32_t (direct_mode) (VG_State* v, uint32_t red, uint32_t green, uint32_t blue);

/**
 * @warning Auxiliary function for the lab 5.
 * @brief Normalize a color to the indexed mode.
 * 
 * @param v Pointer to the VG_State
 * @param column Column position of the pixel
 * @param row Row position of the pixel
 * @param step Step to adjust the color
 * @param first First color to be used
 * @param n Number of colors to be used
 * @return Return the color in hexadecimal format
 */
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

/**
 * @brief Populate a sprite with the xpm image.
 * 
 * @param xpm XPM image to be loaded
 * @param img Pointer to the xpm_image_t to be populated
 * @return Return 0 upon success and non-zero otherwise
*/
int (get_sprite) (xpm_map_t xpm, xpm_image_t* img);

/**
 * @brief Draw a sprite on the screen.
 * 
 * @param v Pointer to the VG_State
 * @param img Pointer to the xpm_image_t
 * @param x X coordinate of the top-left corner of the sprite
 * @param y Y coordinate of the top-left corner of the sprite
 * @return Return 0 upon success and non-zero otherwise
*/
int (draw_sprite) (VG_State* v, xpm_image_t* img, uint16_t x, uint16_t y);

#endif /* _LCOM_VIDEO_H_ */
