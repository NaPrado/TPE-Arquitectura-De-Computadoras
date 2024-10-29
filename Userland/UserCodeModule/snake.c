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
#define TOTAL_OF_BLOCKS (BLOCKS_DIM*BLOCKS_DIM)

typedef struct {
    int x;
    int y;
}Block;

static int option=1;
static int lastOption=0;
static char noColisionsP1=1;
static char noColisionsP2=1;
static char map[TOTAL_OF_BLOCKS];

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

void drawBlock(int blockNumber,int color,int defaultColor){//color if needed, defaultColor if the color is the default
    int y=blockNumber?(blockNumber/BLOCKS_DIM):0;
        drawRectangle(
            (Point){DIM_LEFT_MARGIN+(blockNumber%BLOCKS_DIM)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+y*PIXEL_PER_BLOCK},
            (Point){DIM_LEFT_MARGIN+((blockNumber%BLOCKS_DIM)+1)*PIXEL_PER_BLOCK,DIM_TOP_MARGIN+(y+1)*PIXEL_PER_BLOCK},
            defaultColor?getColorBlock(blockNumber):color
            );
}

void fondo(){
    int color=0;
    int fills=0;
    for (int i = 0; i < TOTAL_OF_BLOCKS; i++,color++){
        drawBlock(i,0,1);
    }
}

void drawVoidRectangle(Point topLeft,Point bottomRigth,uint32_t hexcolor,int width){
    drawRectangle(topLeft,(Point){bottomRigth.x,topLeft.y+width},hexcolor);
    drawRectangle((Point){topLeft.x,bottomRigth.y-width},bottomRigth,hexcolor);
    drawRectangle((Point){topLeft.x,topLeft.y+width},(Point){topLeft.x+width,bottomRigth.y-width},hexcolor);
    drawRectangle((Point){bottomRigth.x-width,topLeft.y+width},(Point){bottomRigth.x,bottomRigth.y-width},hexcolor);
}

void setDefaults(){
    option=1;
    lastOption=0;
    noColisionsP1=1;
    noColisionsP2=1;
    for (int i = 0; i < TOTAL_OF_BLOCKS; i++)
    {
        map[i]=0;
    }
}

void drawselection(){
    drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(option-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+option*3*CHAR_HEIGHT},0xFF0000,3);
    if (lastOption!=0)
    {
        drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(lastOption-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+lastOption*3*CHAR_HEIGHT},0xD0B000,3);
    }    
    lastOption=option;
}
Block getBlockPosition(int blockNumber){
    int x=blockNumber%BLOCKS_DIM;
    int y=blockNumber/BLOCKS_DIM;
    return (Block){x,y};
}

void printOptions(){
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

void cleanOptions(){
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0x000000);
}

void startCount(){
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

int setColisions(Snake* snake){
    if (snake->player==1){
            noColisionsP1=0;
    }else{
        noColisionsP2=0;
    }
}

void actualizeSnakeAndCheckColisions(Snake* snake){
    int nextHeadIndex=snake->head+1;
    nextHeadIndex=nextHeadIndex%TOTAL_OF_BLOCKS;
    Block head=getBlockPosition(snake->body[snake->head]);
    if (snake->dir==UP){
        if (head.y==0){
            setColisions(snake);
            return;
        }
        snake->body[nextHeadIndex]=snake->body[snake->head]-BLOCKS_DIM;
    }else if (snake->dir==DOWN){
        if(head.y==BLOCKS_DIM-1){
            setColisions(snake);
            return;
        }
        snake->body[nextHeadIndex]=snake->body[snake->head]+BLOCKS_DIM;
    }else if (snake->dir==LEFT){
        if (head.x==0){
            setColisions(snake);
            return;
        }
        snake->body[nextHeadIndex]=snake->body[snake->head]-1;
    }else if (snake->dir==RIGHT){
        if (head.x==BLOCKS_DIM-1){
            setColisions(snake);
            return;
        }
        snake->body[nextHeadIndex]=snake->body[snake->head]+1;
    }
    snake->head=nextHeadIndex;
    if (map[snake->body[snake->head]]!=0){
        setColisions(snake);
        return;
    }
    map[snake->body[snake->head]]=snake->player;
    map[snake->body[snake->tail]]=0;
    drawSnakeHead(snake);
    cleanSnakeTail(snake);
    snake->tail++;
    snake->tail=snake->tail%TOTAL_OF_BLOCKS;
}

// void controls(Snake snake1,Snake snake2,char c){
//     if ((c=='W' || c=='w')&& snake1.dir!=DOWN){
//         snake1.dir=UP;
//     }else if ((c=='S' || c=='s')&& snake1.dir!=UP){
//         snake1.dir=DOWN;
//     }else if ((c=='A' || c=='a')&& snake1.dir!=RIGHT){
//         snake1.dir=LEFT;
//     }else if ((c=='D' || c=='d')&& snake1.dir!=LEFT){
//         snake1.dir=RIGHT;
//     }  
//     if (c=='I' || c=='i'){
//         snake2.dir=UP;
//     }else if (c=='K' || c=='k'){
//         snake2.dir=DOWN;
//     }else if (c=='J' || c=='j'){
//         snake2.dir=LEFT;
//     }else if (c=='L' || c=='l'){
//         snake2.dir=RIGHT;
//     }    
// }
void drawFullSnakeAtFirst(Snake* snake){
    drawBlock(snake->body[snake->head],snake->color,0);
    drawBlock(snake->body[snake->tail],snake->color,0);
}

void drawSnakeHead(Snake *snake){
    drawBlock(snake->body[snake->head],snake->color,0);
}

void cleanSnakeTail(Snake* snake){
    drawBlock(snake->body[snake->tail],0,1);
}

void snakeSetter(Snake* snake,int headBlock, int tailBlock){
    snake->body[1]=headBlock;
    snake->body[0]=tailBlock;
    map[snake->body[snake->head]]=snake->player;
    map[snake->body[snake->tail]]=snake->player;
    
}
void startGame(){
    Snake p1={.body={0},.head=1,.tail=0,.length=2,.player=1,.dir=RIGHT,.color=0x00FF50};
    snakeSetter(&p1,204,203);
    drawFullSnakeAtFirst(&p1);
    Snake p2={.body={0},.head=1,.tail=0,.length=2,.player=2,.dir=LEFT,.color=0xFF5050};
    if (option==2){
        snakeSetter(&p2,215,216);
        drawFullSnakeAtFirst(&p2);
    }
    do{
        sleep(9);
        actualizeSnakeAndCheckColisions(&p1);
        if (option==2){
            actualizeSnakeAndCheckColisions(&p2);
        }
    } while (noColisionsP1 && noColisionsP2);
}

void setWinner(){
    setZoom(2);
    setFontColor(0xFF0000);
    setBackGroundColor(0x000000);
    setCursor(25,1);
    if (!noColisionsP1 && !noColisionsP2){
        setCursor(28,1);
        print("Draw!");
    }else if (!noColisionsP1){
        print("Player 2 wins!");
    }else if (!noColisionsP2){
        print("Player 1 wins!");
    }
}
void gameOver(){
    setZoom(2);
    setFontColor(0xFF0000);
    setBackGroundColor(0x000000);
    setCursor(25,1);
    print("Game Over!");
}

void selector(){
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
        startGame();//inicia la partida
        if (option==2){
            setWinner();
        }else if (option==1){
            gameOver();
        }
        setCursor(18,45);
        print ("Press any key to continue");
        getChar();
        setCursor(18,45);
        print ("                         ");
        setCursor(25,1);
        print("            ");
        fondo();
    }else if (option==3){
        cleanScreen();//en caso de seleccionar EXIT limpia la pantalla
    }
}


int chooseOptions(){
    setDefaults();//reinicio de variables
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0xD0B000);//dibuja el marco del menu
    printOptions();//imprime las opciones P1|P2|EXIT
    selector();
}

void snake(){
    margen();//dibuja un marco negro para limpiar la pantalla
    fondo();//dibuja el mantel
    while (option!=3){//reinicia las partidas
        chooseOptions();//selector de opciones P1|P2|EXIT
    }
    setDefaults();
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