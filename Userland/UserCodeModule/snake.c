#include <snake.h>
#include <libc.h>
#define DIM_X 1024
#define DIM_Y 768
#define COLOR1 0x2474FF
#define COLOR2 0x44B6FF

#define PIXEL_PER_BLOCK 32
#define BLOCKS_DIM 20
#define DIM_LEFT_MARGIN (((DIM_X)-(PIXEL_PER_BLOCK*BLOCKS_DIM))/2)
#define DIM_RIGHT_MARGIN DIM_X-DIM_LEFT_MARGIN
#define DIM_TOP_MARGIN (((DIM_Y)-(PIXEL_PER_BLOCK*BLOCKS_DIM))/2)
#define DIM_BOTTOM_MARGIN DIM_Y-DIM_TOP_MARGIN


#define CHAR_WIDTH 8

margen(){
    drawRectangle((Point){0,0},(Point){DIM_LEFT_MARGIN,DIM_Y},white);
    drawRectangle((Point){DIM_RIGHT_MARGIN,0},(Point){DIM_X,DIM_Y},white);
    drawRectangle((Point){DIM_LEFT_MARGIN,0},(Point){DIM_RIGHT_MARGIN,DIM_TOP_MARGIN},white);
    drawRectangle((Point){DIM_LEFT_MARGIN,DIM_BOTTOM_MARGIN},(Point){DIM_RIGHT_MARGIN,DIM_Y},white);

}

fondo(){
    int color=0;
    int fills=0;
    for (int i = 0; i < BLOCKS_DIM*BLOCKS_DIM; i++,color++){
        if (i%BLOCKS_DIM==0 && i!=0){
            color++;
        }
        int y=i?(i/BLOCKS_DIM):0;
        drawRectangle(
            (Point){DIM_LEFT_MARGIN+(i%BLOCKS_DIM)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+y*PIXEL_PER_BLOCK},
            (Point){DIM_LEFT_MARGIN+((i%BLOCKS_DIM)+1)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+(y+1)*PIXEL_PER_BLOCK},
            color%2?COLOR1:COLOR2
            );
    }
    
}

snakeMenu(){
    margen();
    fondo();
}

snake(){
    snakeMenu();
    sleep(36);
}