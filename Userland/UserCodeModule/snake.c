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

typedef struct {
    int x;
    int y;
}Block;

static int option=1;
static int lastOption=0;

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
        int y=i?(i/BLOCKS_DIM):0;
        drawRectangle(
            (Point){DIM_LEFT_MARGIN+(i%BLOCKS_DIM)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+y*PIXEL_PER_BLOCK},
            (Point){DIM_LEFT_MARGIN+((i%BLOCKS_DIM)+1)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+(y+1)*PIXEL_PER_BLOCK},
            getColorBlock(i)
            );
    }
}

drawVoidRectangle(Point topLeft,Point bottomRigth,uint32_t hexcolor,int width){
    drawRectangle(topLeft,(Point){bottomRigth.x,topLeft.y+width},hexcolor);
    drawRectangle((Point){topLeft.x,bottomRigth.y-width},bottomRigth,hexcolor);
    drawRectangle((Point){topLeft.x,topLeft.y+width},(Point){topLeft.x+width,bottomRigth.y-width},hexcolor);
    drawRectangle((Point){bottomRigth.x-width,topLeft.y+width},(Point){bottomRigth.x,bottomRigth.y-width},hexcolor);
}

setDefaults(){
    option=1;
    lastOption=0;
}

void drawselection(){
    drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(option-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+option*3*CHAR_HEIGHT},0xFF0000,3);
    if (lastOption!=0)
    {
        drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(lastOption-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+lastOption*3*CHAR_HEIGHT},0xD0B000,3);
    }    
    lastOption=option;
}
//testear
Block getBlockPosition(int blockNumber){
    int x=blockNumber%BLOCKS_DIM;
    int y=blockNumber/BLOCKS_DIM;
    return (Block){x,y};
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
}

void cleanScreen(){
    drawRectangle((Point){0,0},(Point){DIM_X,DIM_Y},0x000000);
}

cleanOptions(){
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0x000000);
}

startCount(){
    setZoom(2);
    setFontColor(0xFF0000);
    setBackGroundColor(0x000000);
    char c='3';
    for (int i = 3; 0 < i; i--)
    {
        setCursor(31,1);
        putChar(c);
        sleep(18);
        c--;
        //doSound(a);
    }
    setCursor(30,1);
    print("GO!");
    sleep(9);
    setCursor(30,1);
    print("   ");
}

// drawSnake(Snake snake){
//     for (int i = 0; i < snake.length; i++)
//     {
//         Block block=getBlockPosition(snake.body[i]);
//         drawRectangle(
//             (Point){DIM_LEFT_MARGIN+block.x*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+block.y*PIXEL_PER_BLOCK},
//             (Point){DIM_LEFT_MARGIN+(block.x+1)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+(block.y+1)*PIXEL_PER_BLOCK},
//             snake.color
//         );
//     }
// }
//testear
colisions(Snake snake,char map[BLOCKS_DIM][BLOCKS_DIM]){
    Block head=getBlockPosition(snake.body[snake.head]);
    if (head.x<0 || head.x>=BLOCKS_DIM || head.y<0 || head.y>=BLOCKS_DIM)
    {
        return 1;
    }
    if (map[head.y][head.x])
    {
        return 1;
    }
    map[head.y][head.x]=1;
    return 0;
}

void startGame(char map[BLOCKS_DIM][BLOCKS_DIM]){
    Snake p1={
        .body={},
        .head=0,
        .tail=0,
        .length=0,
        .dir=RIGHT,
        .color=0x00FF50
    };
    Snake p2={
        .body={},
        .head=0,
        .tail=0,
        .length=0,
        .dir=LEFT,
        .color=0xFF5050
    };
    //TODO: set the initial position of the snake/s

    char noColisions=1;
    do
    {
        //faltan cosas
        colisions(p1,map);
        if (option==2){
            colisions(p2,map);
        }
        //faltan controles
        //drawSnake(p1);
        //drawSnake(p2);
    } while (/*noColisions*/0);
    
}

selector(char map[BLOCKS_DIM][BLOCKS_DIM]){
    drawselection();
    char c=0;
    do
    {
        if ((c == 'W' || c == 'w' || c == 'I' || c == 'i')&& option>1){
            option--;
            drawselection();
        }else if ((c == 'S' || c == 's' || c == 'K' || c == 'k')&& option<3){
            option++;
            drawselection();
        }
        c=getChar();
        //doSound(a);
    } while (c!='\n');
    if (option==1 || option==2){
        cleanOptions(); //limpia el menu
        startCount();//contador de inicio para la partida
        startGame(map);//inicia la partida
    }else if (option==3){
        cleanScreen();//en caso de seleccionar EXIT limpia la pantalla
    }
}


int chooseOptions(char map[BLOCKS_DIM][BLOCKS_DIM]){
    setDefaults();//reinicio de variables
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0xD0B000);//dibuja el marco del menu
    printOptions();//imprime las opciones P1|P2|EXIT
    selector(map);
}

snake(){
    margen();//dibuja un marco negro para limpiar la pantalla
    fondo();//dibuja el mantel
    while (option!=3){//reinicia las partidas
        char map[BLOCKS_DIM][BLOCKS_DIM]={0};//inicializa el mapa para las colisiones, esta dentro del bucle para reiniciar las partidas
        chooseOptions(map);//selector de opciones P1|P2|EXIT
    }
    cleanScreen();
}
    /*

    Player 1 controlers
      W
    A S D

    Player 2 controlers
      I
    J K L

    */