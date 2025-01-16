#include <gnaGL.h>

void drawRectangle(Point topLeft, Point downRigth, uint32_t color) {
	sys_call(DRAW_RECTANGLE, (uint64_t)&topLeft, (uint64_t)&downRigth, (uint64_t)color, 0);
}

void drawVoidRectangle(Point topLeft,Point bottomRigth,uint32_t hexcolor,int width){
    drawRectangle(topLeft,(Point){bottomRigth.x,topLeft.y+width},hexcolor);
    drawRectangle((Point){topLeft.x,bottomRigth.y-width},bottomRigth,hexcolor);
    drawRectangle((Point){topLeft.x,topLeft.y+width},(Point){topLeft.x+width,bottomRigth.y-width},hexcolor);
    drawRectangle((Point){bottomRigth.x-width,topLeft.y+width},(Point){bottomRigth.x,bottomRigth.y-width},hexcolor);
}