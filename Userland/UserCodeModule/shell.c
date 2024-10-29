#include <shell.h>
#include <libc.h>
#include <stdint.h>
#include <snake.h>

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)

#define COMMAND_LINE_X (4-(zoom/3))                 // Pos de x de la linea de comandos en base al zoom
#define COMMAND_LINE_Y (DIM_CHAR_Y-(4*zoom))        // Pos de y de la linea de comandos en base al zoom

static void inicializeShell();
static void getCommand();
static void doCommand();
static void cleanCommand();
static void printCommands();
static void cleanScreen();

static int zoom = 1;

static char exit = 0;
static const int command_dim = DIM_CHAR_X*2 - 8;  // maximo tamaño de comando, sacando margenes 
static int command_size = 0;
static char command[(DIM_CHAR_X-4)*2];              // buffer de commando escrito
static char response[DIM_CHAR_X*2];                 // buffer de respuesta de commando
static char commandDone = 0;                        // flag para checkear si un comando se realizo (no prints de mas)

void shell() {

    while (!exit) {
        inicializeShell();
        getCommand();
        doCommand();
        if (commandDone) {
            printCommands();
        }
        cleanCommand();
	}
    cleanScreen();
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
    print("exited");
}

void getCommand() {
    char c;
    do {
        c = getChar();
        if (c != '\b') {
            command[command_size++] = c;
        } else {
            command_size -= command_size>0 ? 1 : 0;  // le resto solo si es mayor a 0
            command[command_size] = ' ';
        }
        setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
        print(command);
    } while (c != '\n' && command_size < (command_dim-1)/zoom - 1);
    command[command_size-1] = '\0';
}

static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int color_index = 0;

static int fullLines = 0;
static int actual_Color=white;

void doCommand() {
    if (command[0] != '\0' && command[0] != '\n') {
        commandDone = 1;
        if (strCaseCmp(command, "color") == 0) {
            actual_Color=color[color_index];
            setFontColor(actual_Color);
            color_index = (color_index+1)%21;
            strCpy("New color setted", response);
        } else if (strCaseCmp(command, "date")==0) {
            //time * t = getTime();
            //programTime();
            strCpy(getTime(), response);
        }else if (strCaseCmp(command, "rec")==0){
            Point p1 = {100, 100};
            Point p2 = {200, 200};
            drawRectangle(p1, p2, 0x00FF00);
            strCpy("Rectangle drawn", response);
        } else if (strCaseCmp(command, "help")==0) {
            strCpy("Help", response);
        } else if (strCaseCmp(command, "zoom in") == 0) {
            if (zoom <= 3) { 
                cleanScreen();
                strCpy("Zoomed in", response);
                setZoom(++zoom);
            } else {
                strCpy("Max zoom possible", response);
            }
        } else if (strCaseCmp(command, "zoom out") == 0) {
            if (zoom > 1) { 
                cleanScreen();
                strCpy("Zoomed out", response);
                setZoom(--zoom);
            } else {
                strCpy("Min zoom possible", response);
            }
        } else if (strCaseCmp(command, "snake")==0) {
            snake();
            setZoom(zoom);
            setBackGroundColor(black);
            setFontColor(actual_Color);
            strCpy("Snake exited", response);
            cleanScreen();
        } else if (strCaseCmp(command, "exit")==0) {
            strCpy("Exit", response);
            exit = 1;
        } else {
            strCpy("Command not found", response);
        }
    } else {
        commandDone = 0;
    }
}

void cleanCommand() {
    for (int i = 0; i < command_size; i++) {
        command[i] = '\0';
    }
    command_size = 0;
    setCursor(COMMAND_LINE_X-2, COMMAND_LINE_Y);
}

void printCommands() {

    drawRectangle((Point){1, (COMMAND_LINE_Y-4*zoom)*CHAR_HEIGHT}, (Point){DIM_X, DIM_Y-2*zoom*CHAR_HEIGHT}, 0x000000);
    
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y-4*zoom);
    print(command);

    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y-2*zoom);
    print(response);

}


void inicializeShell() {
    setCursor(COMMAND_LINE_X-2, COMMAND_LINE_Y);
    setFontColor(white);
    print("> ");
}

void cleanScreen() {
    drawRectangle((Point){1, (COMMAND_LINE_Y-4*zoom)*CHAR_HEIGHT}, (Point){DIM_X, DIM_X}, 0x000000);
}
