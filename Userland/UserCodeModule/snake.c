#include <snake.h>
#include <libc.h>
#define DIM_X 1024
#define DIM_Y 768
#define COLOR1 0x2474FF
#define COLOR2 0x44B6FF
#define APPLE_COLOR 0xFF0000
#define COLOR_SNAKE_1 0x00FF00
#define COLOR_SNAKE_2 0x9F009F
#define MENU_BACKGROUND_COLOR 0xD0B000
#define FONT_COLOR 0xFF0000

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

#define TOTAL_OF_BLOCKS (BLOCKS_DIM*BLOCKS_DIM)

typedef struct {
    int x;
    int y;
}Block;

static int option=1;
static int lastOption=0;
static char noColisionsP1=1;
static char noColisionsP2=1;
static char noWinner=1;
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
    noWinner=1;
    for (int i = 0; i < TOTAL_OF_BLOCKS; i++)
    {
        map[i]=0;
    }
}

void drawselection(){
    drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(option-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+option*3*CHAR_HEIGHT},FONT_COLOR,3);
    if (lastOption!=0)
    {
        drawVoidRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN+(lastOption-1)*3*CHAR_HEIGHT},(Point){MENU_RIGHT_MARGIN,MENU_TOP_MARGIN+lastOption*3*CHAR_HEIGHT},MENU_BACKGROUND_COLOR,3);
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
    setFontColor(FONT_COLOR);
    setBackGroundColor(MENU_BACKGROUND_COLOR);
    setCharCursor(2,5);
    print("1P");
    setCharCursor(2,8);
    print("2P");
    setCharCursor(2,11);
    print("EXIT");    
}

void cleanOptions(){
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},0x000000);
}

void startCount(){
    setZoom(2);
    setFontColor(FONT_COLOR);
    setBackGroundColor(black);
    char c='3';
    for (int i = 3; 0 < i; i--)
    {
        setCharCursor(31,1);
        putChar(c);
        sleep(18);
        c--;
        //doSound(a);
    }
    setCharCursor(30,1);
    print("GO!");
    sleep(9);
    setCharCursor(30,1);
    print("   ");
}

int setColisions(Snake* snake){
    if (snake->player==1){
            noColisionsP1=0;
    }else{
        noColisionsP2=0;
    }
}

drawApple(){
    int apple=randInt(0,TOTAL_OF_BLOCKS-1);
    while (map[apple]!=0)
    {
        apple=randInt(0,TOTAL_OF_BLOCKS-1);
    }
    map[apple]=2;
    drawBlock(apple,APPLE_COLOR,0);
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
    if (map[snake->body[snake->head]]==1){
        setColisions(snake);
        return;
    }
    if (map[snake->body[snake->head]]==2){
        snake->length++;
        map[snake->body[snake->head]]=1;
        drawApple();
        drawSnakeHead(snake);
        if (snake->length==TOTAL_OF_BLOCKS){
            noWinner=0;
            return;
        }
    }else{
        map[snake->body[snake->head]]=1;
        map[snake->body[snake->tail]]=0;
        drawSnakeHead(snake);
        cleanSnakeTail(snake);
        snake->tail++;
        snake->tail=snake->tail%TOTAL_OF_BLOCKS;
    }
}

void controls(Snake* snake1,Snake* snake2){
    char c=getKey();
    Direction dir1=snake1->dir;
    Direction dir2=snake2->dir;
    while (c!=-2){
        if ((c=='W' || c=='w') && dir1!=DOWN){
            snake1->dir=UP;
        }else if ((c=='S' || c=='s') && dir1!=UP){
            snake1->dir=DOWN;
        }else if ((c=='A' || c=='a') && dir1!=RIGHT){
            snake1->dir=LEFT;
        }else if ((c=='D' || c=='d') && dir1!=LEFT){
            snake1->dir=RIGHT;
        }  
        if ((c=='I' || c=='i') && dir2!=DOWN){
            snake2->dir=UP;
        }else if ((c=='K' || c=='k') && dir2!=UP){
            snake2->dir=DOWN;
        }else if ((c=='J' || c=='j') && dir2!=RIGHT){
            snake2->dir=LEFT;
        }else if ((c=='L' || c=='l') && dir2!=LEFT){
            snake2->dir=RIGHT;
        }
        c=getKey();
    }
}

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
    Snake p1={.body={0},.head=1,.tail=0,.length=2,.player=1,.dir=RIGHT,.color=COLOR_SNAKE_1};
    snakeSetter(&p1,204,203);
    drawFullSnakeAtFirst(&p1);
    Snake p2={.body={0},.head=1,.tail=0,.length=2,.player=2,.dir=LEFT,.color=COLOR_SNAKE_2};
    if (option==2){
        snakeSetter(&p2,215,216);
        drawFullSnakeAtFirst(&p2);
    }
    startCount();//contador de inicio para la partida
    drawApple();
    do{
        sleep(9);
        controls(&p1,&p2);
        actualizeSnakeAndCheckColisions(&p1);
        if (option==2){
            actualizeSnakeAndCheckColisions(&p2);
        }
    } while (noColisionsP1 && noColisionsP2 && noWinner);
}

void setWinner(){
    setZoom(2);
    setFontColor(FONT_COLOR);
    setBackGroundColor(black);
    setCharCursor(25,1);
    if (!noColisionsP1 && !noColisionsP2){
        setCharCursor(28,1);
        print("Draw!");
    }else if (!noColisionsP1){
        print("Player 2 wins!");
    }else if (!noColisionsP2){
        print("Player 1 wins!");
    }
}
void gameOverOrWinner(){
    setZoom(2);
    setFontColor(FONT_COLOR);
    setBackGroundColor(black);
    setCharCursor(25,1);
    if (!noWinner){
                print("Winner!");
                return;
    }
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
        startGame();//inicia la partida
        if (option==2){
            setWinner();
        }else if (option==1){
            gameOverOrWinner();
        }
        setCharCursor(18,45);
        print ("Press enter to continue");
        c=getChar();
        while(c!='\n'){
            c=getChar();
        }
        setCharCursor(18,45);
        print ("                         ");
        setCharCursor(25,1);
        print("                ");
        fondo();
    }else if (option==3){
        cleanFullScreen();//en caso de seleccionar EXIT limpia la pantalla
    }
}


int chooseOptions(){
    setDefaults();//reinicio de variables
    drawRectangle((Point){MENU_LEFT_MARGIN,MENU_TOP_MARGIN},(Point){MENU_RIGHT_MARGIN,MENU_BOTTOM_MARGIN},MENU_BACKGROUND_COLOR);//dibuja el marco del menu
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
    cleanFullScreen();
}
    /*

    Player 1 controlers
      W
    A S D

    Player 2 controlers
      I
    J K L

    */