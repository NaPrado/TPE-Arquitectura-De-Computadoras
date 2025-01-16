#ifndef _GNAGL_H_
#define _GNAGL_H_
#include <libasm.h>
#define DRAW_RECTANGLE 9

typedef struct {
	int x;
	int y;
}Point;

void drawRectangle(Point topLeft, Point downRigth, uint32_t color);

void drawVoidRectangle(Point topLeft,Point bottomRigth,uint32_t hexcolor,int width);

#endif