#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
#include <stdint.h>
// Function declarations and macros for the video driver

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

typedef struct {
	int x;
	int y;
}Point;
/**
 * @brief Draws a pixel on the screen at the specified coordinates with the given color.
 *
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void drawRectangle(Point* topLeft, Point* downRigth, uint32_t c);

void drawVoidRectangle(Point* topLeft, Point* downRigth, uint32_t c, int mult);

void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor, int mult);

void putchar(char c, int x, int y);

void redrawScreen();

#endif // VIDEODRIVER_H