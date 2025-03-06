#include <tetris.h>
#include <gnaGL.h>
#include <random.h>
#include <sounds.h>
#include <libc.h>
#define DIM_X 1024
#define DIM_Y 768
#define PIXEL_PER_BLOCK 32
#define BLOCKS_DIMX 10
#define BLOCKS_DIMY 20
#define DIM_LEFT_MARGIN (((DIM_X)-(PIXEL_PER_BLOCK*BLOCKS_DIMX))/2)
#define DIM_RIGHT_MARGIN DIM_X-DIM_LEFT_MARGIN
#define DIM_TOP_MARGIN (((DIM_Y)-(PIXEL_PER_BLOCK*BLOCKS_DIMY))/2)
#define DIM_BOTTOM_MARGIN DIM_Y-DIM_TOP_MARGIN
#define EDGE_COLOR1 0x505050
#define EDGE_COLOR2 0x303030
#define EDGE_COLOR3 0x101010
#define EDGE_LOGO_COLOR 0x0060FF
#define BACKGROUND_COLOR 0x000060
#define PULLED_BACKGROUND_COLOR 0x0000A0
#define SHINE_COLOR 0xFFFFFF
#define Z_COLOR 0xFF0000
#define S_COLOR 0x00FF00
#define O_COLOR 0xFFFF00
#define I_COLOR 0x00FFFF
#define J_COLOR 0x0000FF
#define L_COLOR 0xFF7700
#define T_COLOR 0X770077

typedef enum{
    Z=0,S,O,I,J,L,T
}Shape;

static Shape shapes[]={Z,S,O,I,J,L,T};
#define SHAPES_DIM 7

typedef struct {
    int16_t x;
    int16_t y;
}Position;

typedef enum{
    top=0,
    right,
    bottom,
    left
}Dir;

typedef struct{
    uint8_t dir;
    Shape shape; 
    Position* pos;
    uint8_t fix;
}Piece;

static Piece piece(Dir dir,Shape shape,Position* pos,uint8_t fix){
    Piece p;
    p.dir=dir;
    p.shape=shape;
    p.pos=pos;
    p.fix=fix;
    return p;
}
static Shape nextPiece;

static void drawNextPiece();
static void cleanNextPiece();

static Piece setNextPiece(){
    cleanNextPiece();
    nextPiece=shapes[randInt(0,SHAPES_DIM-1)];
    drawNextPiece();
}

static Piece actualPiece;
static Shape holdedShape=0;
//0 is empty, 1 is fixed, 2 is pullable
static int holdRealiced=0;
Position actualPos = {3, 0};
static initActualPiece(Shape s){
    actualPos.y=0;
    actualPos.x=3;
    actualPiece=piece(0,s,&actualPos,0);
}

static void drawHoldedPiece(Shape p);

Sound tetrisMusic[]={
    (Sound){E5,5},(Sound){B4,3},(Sound){C5,3},
    (Sound){D5,5},(Sound){C5,3},(Sound){B4,3},
    (Sound){A4,5},(Sound){0,0},(Sound){A4,3},
    (Sound){C5,3},(Sound){E5,3},(Sound){D5,3},
    (Sound){C5,3},(Sound){B4,5},(Sound){0,0},
    (Sound){B4,3},(Sound){C5,3},(Sound){D5,5},
    (Sound){E5,5},(Sound){C5,5},(Sound){A4,5},
    (Sound){0,0},(Sound){A4,10},(Sound){D5,5},
    (Sound){F5,5},(Sound){A5,5},(Sound){G5,3},
    (Sound){F5,3},(Sound){E5,7},(Sound){C5,3},
    (Sound){E5,5},(Sound){D5,3},(Sound){C5,3},
    (Sound){B4,5},(Sound){0,0}, (Sound){B4,3},
    (Sound){C5,3},(Sound){D5,5},(Sound){E5,5},
    (Sound){C5,5},(Sound){A4,5},(Sound){0,0},
    (Sound){A4,8},(Sound){E5,8},(Sound){C5,8},
    (Sound){D5,8},(Sound){B4,8},(Sound){C5,8},
    (Sound){A4,8},(Sound){415,8},(Sound){B4,8},
    (Sound){E5,8},(Sound){C5,8},(Sound){D5,8},
    (Sound){B4,8},(Sound){C5,5},(Sound){E5,5},
    (Sound){A5,5},(Sound){830,10},(Sound){0,0}
};

static uint32_t colisionMap[10][20]={
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};
static void cleanColisionMap(){
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            colisionMap[j][i]=0;
        }
        
    }
    
}

//Unused
static void drawLogo(){
    drawVoidRectangle((Point){(DIM_LEFT_MARGIN/5)*1,(DIM_LEFT_MARGIN/5)*1},(Point){(DIM_LEFT_MARGIN/5)*4,(DIM_LEFT_MARGIN/5)*2},EDGE_LOGO_COLOR,5);
    drawVoidRectangle((Point){(DIM_LEFT_MARGIN/5)*2,(DIM_LEFT_MARGIN/5)*2},(Point){(DIM_LEFT_MARGIN/5)*3,(DIM_LEFT_MARGIN/5)*3},EDGE_LOGO_COLOR,5);
    drawRectangle((Point){(DIM_LEFT_MARGIN/5)*1+5,(DIM_LEFT_MARGIN/5)*1+5},(Point){(DIM_LEFT_MARGIN/5)*4-5,(DIM_LEFT_MARGIN/5)*2-5},BACKGROUND_COLOR);
    drawRectangle((Point){(DIM_LEFT_MARGIN/5)*2+5,(DIM_LEFT_MARGIN/5)*2-5},(Point){(DIM_LEFT_MARGIN/5)*3-5,(DIM_LEFT_MARGIN/5)*3-5},BACKGROUND_COLOR);
    setZoom(3);
    setBackGroundColor(BACKGROUND_COLOR);
    setCursor((DIM_LEFT_MARGIN/5)*1+5+5,(DIM_LEFT_MARGIN/5)*1+5+2);
    setFontColor(Z_COLOR);
    print(" T");
    setFontColor(L_COLOR);
    print("E");
    setFontColor(O_COLOR);
    print("T");
    setFontColor(S_COLOR);
    print("R");
    setFontColor(I_COLOR);
    print("I");
    setFontColor(T_COLOR);
    print("S");
}

static uint32_t darkColor(uint32_t color){
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    r/=3;
    g/=3;
    b/=3;
    return (r << 16) | (g << 8) | b;
}

static void unDrawAsBlackBlock(uint32_t x,uint32_t y){
    drawRectangle((Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*x,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*y},(Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*(x+1),DIM_TOP_MARGIN+PIXEL_PER_BLOCK*(y+1)},black);
}

static void unDrawBlock(uint32_t x,uint32_t y){
    drawRectangle((Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*x,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*y},(Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*(x+1),DIM_TOP_MARGIN+PIXEL_PER_BLOCK*(y+1)},BACKGROUND_COLOR);
}

static void drawBlock(uint32_t x,uint32_t y,uint32_t color){
    drawRectangle((Point){DIM_LEFT_MARGIN+4+PIXEL_PER_BLOCK*x,DIM_TOP_MARGIN+4+PIXEL_PER_BLOCK*y},(Point){DIM_LEFT_MARGIN-4+PIXEL_PER_BLOCK*(x+1),DIM_TOP_MARGIN-4+PIXEL_PER_BLOCK*(y+1)},color);
    drawVoidRectangle((Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*x,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*y},(Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*(x+1),DIM_TOP_MARGIN+PIXEL_PER_BLOCK*(y+1)},darkColor(color),4);
    drawRectangle((Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*x+4,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*y+4},(Point){DIM_LEFT_MARGIN+PIXEL_PER_BLOCK*x+4+5,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*y+4+5},SHINE_COLOR);
}

static void drawTetrisContext(){
    drawRectangle((Point){DIM_LEFT_MARGIN-3-5-3,DIM_TOP_MARGIN-3-5-3-PIXEL_PER_BLOCK},(Point){DIM_RIGHT_MARGIN+3+5+3,DIM_TOP_MARGIN-3-5-3},black);
    drawVoidRectangle((Point){DIM_LEFT_MARGIN-3-5-3,DIM_TOP_MARGIN-3-5-3},(Point){DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN+3+5+3},EDGE_COLOR3,3);
    drawVoidRectangle((Point){DIM_LEFT_MARGIN-3-5,DIM_TOP_MARGIN-3-5},(Point){DIM_RIGHT_MARGIN+3+5,DIM_BOTTOM_MARGIN+3+5},EDGE_COLOR2,5);
    drawVoidRectangle((Point){DIM_LEFT_MARGIN-3,DIM_TOP_MARGIN-3},(Point){DIM_RIGHT_MARGIN+3,DIM_BOTTOM_MARGIN+3},EDGE_COLOR1,3);
    drawRectangle((Point){DIM_LEFT_MARGIN,DIM_TOP_MARGIN},(Point){DIM_RIGHT_MARGIN,DIM_BOTTOM_MARGIN},BACKGROUND_COLOR);
    setFontColor(SHINE_COLOR);
    setZoom(2);
    setBackGroundColor(black);
    setCursor(DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK*2.5,DIM_TOP_MARGIN-PIXEL_PER_BLOCK);
    print("NEXT");
    drawVoidRectangle((Point){DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK,DIM_TOP_MARGIN},(Point){DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK*6,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*6},SHINE_COLOR,3);
    setCursor(DIM_LEFT_MARGIN-(PIXEL_PER_BLOCK*2.5)-BASE_CHAR_WIDTH*4*2,DIM_TOP_MARGIN-PIXEL_PER_BLOCK);
    print("HOLD");
    drawVoidRectangle((Point){DIM_LEFT_MARGIN-PIXEL_PER_BLOCK*6,DIM_TOP_MARGIN},(Point){DIM_LEFT_MARGIN-PIXEL_PER_BLOCK,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*6},SHINE_COLOR,3);
}
typedef uint8_t Dirs[4][4];

static uint32_t getColor(Shape shape){
    uint32_t colors[]={Z_COLOR,S_COLOR,O_COLOR,I_COLOR,J_COLOR,L_COLOR,T_COLOR};
    return colors[shape];
}

static void printPatern(Dirs pat,Position pos,Shape shape){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (pat[j][i]){
                drawBlock(pos.x+i,pos.y+j,getColor(shape));
            }
        }
    }  
}

static void cleanAsBlackPatern(Dirs pat,Position pos,Shape shape){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (pat[j][i]){
                unDrawAsBlackBlock(pos.x+i,pos.y+j);
            }
        }
    }
}

static void cleanPatern(Dirs pat,Position pos,Shape shape){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (pat[j][i]){
                unDrawBlock(pos.x+i,pos.y+j);
            }
        }
    }
}
static uint8_t patterns[7][4][4][4]={
    {
        //Z Pattern
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {0,0,0,0}
        },{
            {0,1,1,0},
            {0,0,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },{
        //S Pattern
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },{
            {0,0,1,1},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        }
    },{
        //O Pattern
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },{
        //I Patern
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },{
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },{ 
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },{    
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },{
        //J Pattern
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        },{
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },{
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },{
        //L Pattern
        {
            {1,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,0,0,0}
        },{
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },{
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },{
        //T Pattern
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },{
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },{
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },{
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
    }
};
Dirs *getPattern(int dir, Shape shape) {
    return patterns[shape] + (dir%4);
}

static Shape getRandShape(){
    return randInt(0,6);
}
static uint8_t wallColision(Piece p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (( p.pos->x+i<0 || p.pos->x+i>9) && (*getPattern(p.dir,p.shape))[j][i]){
                return 1;
            }
        }
    }
    return 0;
}   
static uint8_t checkVerticalColision(Piece p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if ((*getPattern(p.dir,p.shape))[j][i]){
                if (p.pos->y+j>19){
                    return 1;
                }
                if (colisionMap[p.pos->y+j][p.pos->x+i]){
                    return 1;
                } 
            }
        }
    }
    return 0;

}
static uint8_t checkHorizontalColision(Piece p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (colisionMap[p.pos->y+j][p.pos->x+i] && (*getPattern(p.dir,p.shape))[j][i]){
                return 1;
            }
        }
    }
    return 0;
}
//esta funcion es cuando se queda quieta la pieza
static void actualizeMapColision(Position pos,Dirs pat,uint32_t color){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (pat[j][i]){
                colisionMap[pos.y+j][pos.x+i]=color;
            }
        }
    }
}

static uint8_t hasBeenAColision(Piece p){
    return checkVerticalColision(p) || checkHorizontalColision(p) || wallColision(p);
}


static void abstractDrawCleanPiece(Piece p, uint8_t boolDrawOrClean){
    void (*func[]) (Dirs,Position,Shape)={cleanPatern,printPatern};
    func[boolDrawOrClean](*getPattern(p.dir,p.shape),*p.pos,p.shape);
}

static void drawPiece(Piece p){
    abstractDrawCleanPiece(p,1);
}
static void cleanNextPiece(){
    Position pos = {12, 1};
    Piece aux=piece(0,nextPiece,&pos,0);
    cleanAsBlackPatern(*getPattern(aux.dir,aux.shape),*aux.pos,aux.shape);
}

static void drawNextPiece(){
    Position pos = {12, 1};
    Piece aux=piece(0,nextPiece,&pos,0);
    abstractDrawCleanPiece(aux,1);
}
static void cleanHoldedPiece(){
    Position pos = {-5, 1};
    Piece aux=piece(0,holdedShape,&pos,0);
    cleanAsBlackPatern(*getPattern(aux.dir,aux.shape),*aux.pos,aux.shape);
}
static void drawVoidHoldedPiece(Shape shape){
    Position pos = {-5, 1};
    Piece aux=piece(0,shape,&pos,0);
    cleanAsBlackPatern(*getPattern(aux.dir,aux.shape),*aux.pos,aux.shape);
}
static void drawHoldedPiece(Shape shape){
    Position pos = {-5, 1};
    Piece aux=piece(0,shape,&pos,0);
    abstractDrawCleanPiece(aux,1);
}
static void cleanPiece(Piece p){
    abstractDrawCleanPiece(p,0);
}

static Piece drawDropPiece(Piece p){
    cleanPiece(p);
    while (!checkVerticalColision(p)){
        p.pos->y++;
    }
    p.pos->y--;
    drawPiece(p);
    actualizeMapColision(*p.pos,*getPattern(p.dir,p.shape),getColor(p.shape));
    p.fix=9;
    return p;
}

static uint8_t drawAndFallPiece(Piece* p){
    cleanPiece(*p);
    p->pos->y++;
    if (checkVerticalColision(*p)){
        p->pos->y--;
        drawPiece(*p);
        return 0;
    }
    drawPiece(*p);
    return 1;
}

static uint8_t colisionDir=0;
static uint8_t noRotate=0;


static const Position kickOffsets[4][5] = {
    {{0, 0}, {1, 0}, {-1, 0}, {0, -1}, {0, 1}},   // Desde 0° a 90°
    {{0, 0}, {-1, 0}, {1, 0}, {0, 1}, {0, -1}},   // Desde 90° a 180°
    {{0, 0}, {1, 0}, {-1, 0}, {0, -1}, {0, 1}},   // Desde 180° a 270°
    {{0, 0}, {-1, 0}, {1, 0}, {0, 1}, {0, -1}}    // Desde 270° a 0°
};

uint8_t rotatePiece(Piece *p, int clockwise) {
    uint8_t originalDir = p->dir;
    uint8_t newDir = (originalDir + (clockwise ? 1 : -1) + 4) % 4;

    for (int i = 0; i < 5; i++) {
        Position newPos = {
            p->pos->x + kickOffsets[originalDir][i].x,
            p->pos->y + kickOffsets[originalDir][i].y
        };
        Piece testPiece = {newDir, p->shape, &newPos, p->fix};

        if (!hasBeenAColision(testPiece)) {
            cleanPiece(*p);
            p->dir = newDir;
            p->pos->x = newPos.x;
            p->pos->y = newPos.y;
            drawPiece(*p);
            return 1;
        }
    }

    return 0;
}

static uint8_t loose=0;

static Piece holdPiece(Shape s){
    holdedShape= s;
    drawHoldedPiece(s);
    holdRealiced=1;
}

static Piece doMovement(Piece p){
    char c;
    Piece newP=piece(p.dir,p.shape,p.pos,p.fix);
    while ((c=getChar())!=-2){
        cleanPiece(p);
        if (c == 'a' || c=='A'){
            //izquierda
            newP.pos->x--;
            if (wallColision(newP) || checkHorizontalColision(newP)){
                newP.pos->x++;
            }else{
                cleanPiece(p);
                drawPiece(newP);
            }
        }else if (c == 'd' || c=='D'){
            //derecha
            newP.pos->x++;
            if (wallColision(newP) || checkHorizontalColision(newP)){
                newP.pos->x--;
            }else{
                cleanPiece(p);
                drawPiece(newP);
            }
        }else if (c == 's' || c=='S'){
            //abajo lento
            newP.pos->y++;
            if (checkVerticalColision(newP)){
                newP.pos->y--;
                newP.fix++;
            }
        }else if (c == 'j' || c=='J'){
            //hold
            if (holdRealiced==0){
                cleanPiece(newP);
                holdPiece(newP.shape);
                initActualPiece(nextPiece);
                setNextPiece();
                return actualPiece;
            }else if (holdRealiced==2){
                cleanPiece(newP);
                cleanHoldedPiece();
                Shape aux=holdedShape;
                holdedShape=p.shape;
                initActualPiece(aux);
                holdPiece(holdedShape);
                holdRealiced=1;
                return actualPiece;
            }
            
        }else if (c == 'k' || c=='K'){
            //rotar izq
            rotatePiece(&newP, 0);
        }else if (c == 'l' || c=='L'){
            //rotar der
            rotatePiece(&newP, 1);
        }else if (c == ' '){
            if (hasBeenAColision(newP)){
                return p;
            }
            //abajo rapido
            newP=drawDropPiece(newP);
            return newP;
        }
        drawPiece(newP);
        p=newP;
    }
    cleanPiece(p);
    newP.pos->y++;
    if (checkVerticalColision(newP)){
                newP.pos->y--;
                newP.fix++;
    }else{
        newP.fix=0;
    }
    drawPiece(newP);
    return newP;
}
static void markLine(uint8_t line){
    for (int i = 0; i < 10; i++){
        drawBlock(i,line,0xFFFFFF);
    }
}

static uint8_t checkLines(){
    uint8_t lines=0;
    for (int i = 0; i < 20; i++){
        uint8_t line=1;
        for (int j = 0; j < 10; j++){
            if (!colisionMap[i][j]){
                line=0;
                break;
            }
        }
        if (line){
            lines++;
            markLine(i);
            sleep(1);
            for (int j = i; j > 0; j--){
                for (int k = 0; k < 10; k++){
                    colisionMap[j][k]=colisionMap[j-1][k];
                }
            }
        }
    }
    return lines;
}

static void redrawMap(){
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 10; j++){
            if (colisionMap[i][j]){
                drawBlock(j,i,colisionMap[i][j]);
            }else{
                unDrawBlock(j,i);
            }
        }
    }
}
static void printWhitePiece(Piece p){
    Dirs* pat=getPattern(p.dir,p.shape);
    Position pos=(*p.pos);
    Shape shape=p.shape;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if ((*pat)[j][i]){
                drawBlock(pos.x+i,pos.y+j,0xFFFFFF);
            }
        }
    }
}
static void undrawMenu(){
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(0x000000);
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)-BASE_CHAR_HEIGHT*2);
    print("Start");
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5));
    print("Hide Controls");
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)+BASE_CHAR_HEIGHT*2);
    print("Exit");
}

static void printCombo(int lines){
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(0xFFFFFF);
    setCursor(DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK*2.5-BASE_CHAR_WIDTH*6,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*6+BASE_CHAR_HEIGHT*9);
    if (lines==1){
        print("Single");
    }else if (lines==2){
        setFontColor(Z_COLOR);
        print("Double");
    }else if (lines==3){
        setFontColor(I_COLOR);
        print("Triple");
    }else if (lines==4){
        setFontColor(Z_COLOR);
        print("T");
        setFontColor(L_COLOR);
        print("E");
        setFontColor(O_COLOR);
        print("T");
        setFontColor(S_COLOR);
        print("R");
        setFontColor(I_COLOR);
        print("I");
        setFontColor(T_COLOR);
        print("S"); 
    }else{
        print("      ");
    }
}

static int32_t setPoints(uint32_t lines, uint32_t points){
    playSound(0,0);
    if (lines==1){
        points+=40;
        playSound(F5,1);
    }else if (lines==2){
        points+=100;
        playSound(F5,1);
        playSound(G5,1);
    }else if (lines==3){
        points+=300;
        playSound(F5,1);
        playSound(G5,1);
        playSound(A5,1);
    }else if (lines==4){
        points+=1200;
        playSound(F5,1);
        playSound(G5,1);
        playSound(A5,1);
        playSound(B5,1);
    }
    playSound(0,0);
    return points; 
}
static void printPoints(uint32_t points){
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(0xFFFFFF);
    setCursor(DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK*2.5-BASE_CHAR_WIDTH*6,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*6+BASE_CHAR_HEIGHT);
    print("Points: ");
    setCursor(DIM_RIGHT_MARGIN+PIXEL_PER_BLOCK*2.5-BASE_CHAR_WIDTH*6,DIM_TOP_MARGIN+PIXEL_PER_BLOCK*6+BASE_CHAR_HEIGHT*3);
    char buffer[10];
    itoa(points,buffer,10,0);
    print(buffer);
}

static void startGame(){
    if (holdRealiced!=0){
        drawVoidHoldedPiece(holdedShape);
    }
    holdRealiced=0;

    //try debugging with an smaller type//////
    uint64_t totalLines=0;////////////////////
    uint64_t speed=0;/////////////////////////
    uint64_t level=0;/////////////////////////
    __uint128_t points=0;/////////////////////
    //////////////////////////////////////////
    uint8_t allowFix=1;                 
    drawTetrisContext();
    cleanColisionMap();
    undrawMenu();
    initActualPiece(getRandShape());
    setNextPiece();
    while (!loose){
        while (actualPiece.fix<=allowFix && !loose){
            drawPiece(actualPiece);
            sleep(10-speed);
            actualPiece=doMovement(actualPiece);
            if(hasBeenAColision(actualPiece)&&actualPiece.pos->y==0){
                loose=1;
            }
        }
        printWhitePiece(actualPiece);
        sleep(1);
        drawPiece(actualPiece);
        holdRealiced=2;
        actualizeMapColision(*actualPiece.pos,*getPattern(actualPiece.dir,actualPiece.shape),getColor(actualPiece.shape));
        int lines=checkLines();
        //could be an error here//////
        totalLines+=lines;          //
        printCombo(lines);          //
        //////////////////////////////
        if(lines){
            redrawMap();
            points=setPoints(lines,points);
            printPoints(points);
        }
        if (totalLines/8>level && speed<9){
            level++;
            speed+=1;
            allowFix+=1?allowFix<9:0;
        }
        
        initActualPiece(nextPiece);
        setNextPiece();
    }
    loose=0;
}





static void showControls(){
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(EDGE_LOGO_COLOR);
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*7*2);
    print("A: Move Left");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*6*2);
    print("D: Move Right");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*5*2);
    print("S: Fall");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*4*2);
    print("J: Hold");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*3*2);
    print("K: Rotate Left");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*2*2);
    print("L: Rotate Right");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*2);
    print("Space: Drop");
}
static void cleanControls(){
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(0x000000);
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*7*2);
    print("A: Move Left");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*6*2);
    print("D: Move Right");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*5*2);
    print("S: Fall");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*4*2);
    print("J: Hold");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*3*2);
    print("K: Rotate Left");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*2*2);
    print("L: Rotate Right");
    setCursor(5+DIM_RIGHT_MARGIN+3+5+3,DIM_BOTTOM_MARGIN-BASE_CHAR_HEIGHT*2);
    print("Space: Drop");
}
static uint8_t showedControls=1;

static void startMenu(){
    printPoints(0);
    setZoom(2);
    setBackGroundColor(0x000000);
    setFontColor(EDGE_LOGO_COLOR);
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)-BASE_CHAR_HEIGHT*2);
    print("Start");
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5));
    print(showedControls?"Show Controls":"Hide Controls");
    setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)+BASE_CHAR_HEIGHT*2);
    print("Exit");
    char c,option=0;
    while ((c=getChar())!='\n'){
        if ((c=='w' || c=='W')){
            option--;
            option=(option+3)%3;
        }else if ((c=='s' || c=='S')){
            option++;
            option=option%3;
        }
        switch (option){
        case 0:
            setFontColor(S_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)-BASE_CHAR_HEIGHT*2);
            print("Start");
            setFontColor(EDGE_LOGO_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5));
            print(showedControls?"Show Controls":"Hide Controls");
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)+BASE_CHAR_HEIGHT*2);
            print("Exit");
            break;
        case 1:
            setFontColor(EDGE_LOGO_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)-BASE_CHAR_HEIGHT*2);
            print("Start");
            setFontColor(S_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5));
            print(showedControls?"Show Controls":"Hide Controls");
            setFontColor(EDGE_LOGO_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)+BASE_CHAR_HEIGHT*2);
            print("Exit");
            break;
        case 2:
            setFontColor(EDGE_LOGO_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)-BASE_CHAR_HEIGHT*2);
            print("Start");
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5));
            print(showedControls?"Show Controls":"Hide Controls");
            setFontColor(S_COLOR);
            setCursor((DIM_LEFT_MARGIN/5)*1,DIM_BOTTOM_MARGIN-(DIM_LEFT_MARGIN/5)+BASE_CHAR_HEIGHT*2);
            print("Exit");
            break;
        default:
            break;
        }
        _hlt();
    }
    switch (option){
    case 0:
        startGame();
        startMenu();
        break;
    case 1:
        if (!showedControls){
            cleanControls();
        }else{
            showControls();
        }
        showedControls=(!showedControls);
        startMenu();
        break;
    case 2:
        break;
    default:
        break;
    }
}


void tetris(){
    pauseResumeMusic(0);
    setBackgroundMusic(tetrisMusic,sizeof(tetrisMusic)/sizeof(Sound));
    drawTetrisContext();
    startMenu();
    cleanFullScreen();
    pauseResumeMusic(1);
}