#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
#include <stdint.h>
// Function declarations and macros for the video driver

/**
 * @brief Draws a pixel on the screen at the specified coordinates with the given color.
 *
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor);

void putchar(char c);

void redrawScreen();

#endif // VIDEODRIVER_H