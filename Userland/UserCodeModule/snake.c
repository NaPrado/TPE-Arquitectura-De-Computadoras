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
#define MENU_LEFT_MARGIN DIM_LEFT_MARGIN/8
#define MENU_RIGHT_MARGIN MENU_LEFT_MARGIN*7
#define MENU_TOP_MARGIN DIM_TOP_MARGIN
#define MENU_BOTTOM_MARGIN DIM_BOTTOM_MARGIN

#define FIRST_PLAYER 1
#define SECOND_PLAYER 2
#define EXIT 3

#define CHAR_HEIGHT 16
#define CHAR_WIDTH 8

void margen(){
    drawRectangle((Point){0,0},(Point){DIM_LEFT_MARGIN,DIM_Y},black);
    drawRectangle((Point){DIM_RIGHT_MARGIN,0},(Point){DIM_X,DIM_Y},black);
    drawRectangle((Point){DIM_LEFT_MARGIN,0},(Point){DIM_RIGHT_MARGIN,DIM_TOP_MARGIN},black);
    drawRectangle((Point){DIM_LEFT_MARGIN,DIM_BOTTOM_MARGIN},(Point){DIM_RIGHT_MARGIN,DIM_Y},black);
}

uint32_t getColorBlock(int blockNumber){
    int fill=blockNumber/BLOCKS_DIM;
    return (blockNumber+fill)%2?COLOR1:COLOR2;
}

void fondo(){
    int color=0;
    int fills=0;
    for (int i = 0; i < BLOCKS_DIM*BLOCKS_DIM; i++,color++){
        // if (i%BLOCKS_DIM==0 && i!=0){
        //     color++;
        // }
        int y=i?(i/BLOCKS_DIM):0;
        drawRectangle(
            (Point){DIM_LEFT_MARGIN+(i%BLOCKS_DIM)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+y*PIXEL_PER_BLOCK},
            (Point){DIM_LEFT_MARGIN+((i%BLOCKS_DIM)+1)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+(y+1)*PIXEL_PER_BLOCK},
            getColorBlock(i)
            // color%2?COLOR1:COLOR2
            );
    }
}

drawVoidRectangle(Point topLeft,Point bottomRigth,uint32_t hexcolor,int width){
    drawRectangle(topLeft,(Point){bottomRigth.x,topLeft.y+width},hexcolor);
    drawRectangle((Point){topLeft.x,bottomRigth.y-width},bottomRigth,hexcolor);
    drawRectangle((Point){topLeft.x,topLeft.y+width},(Point){topLeft.x+width,bottomRigth.y-width},hexcolor);
    drawRectangle((Point){bottomRigth.x-width,topLeft.y+width},(Point){bottomRigth.x,bottomRigth.y-width},hexcolor);
}

static int option=1;
static int lastOption=0;

void drawselection(){
    drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(option-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+option*3*CHAR_HEIGHT},0xFF0000,3);
    if (lastOption!=0)
    {
        drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(lastOption-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+lastOption*3*CHAR_HEIGHT},0xD0B000,3);
    }    
    lastOption=option;
}


printOptions(){
    setZoom(2);
    setFontColor(0xFF0000);
    setBackGroundColor(0xD0B000);
    setCursor(2,5);
    print("1P");
    setCursor(2,8);
    print("2P");
    setCursor(2,11);
    print("EXIT");
    /*

    Player 1 controlers
      W
    A S D

    Player 2 controlers
      I
    J K L

    */
    char c=getChar();
    drawselection();
    while (c!='\n'){
        
        if ((c == 'W' || c == 'w' || c == 'I' || c == 'i')&& option>1){
            option--;
            drawselection();
        }else if ((c == 'S' || c == 's' || c == 'K' || c == 'k')&& option<3){
            option++;
            drawselection();
        }
        //doSound(c);
    } 
    
    
}

int chooseOptions(){
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0xD0B000);
    printOptions();
    //Selector();
}

snakeInit(){
    margen();
    fondo();
    chooseOptions();
}

snake(){
    snakeInit();
    chooseOptions();
    while (1)
    {

        sleep(36);
    }
    
    
}