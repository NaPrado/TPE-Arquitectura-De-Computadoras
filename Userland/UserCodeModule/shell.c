#include <shell.h>
#include <libc.h>
#include <stdint.h>
#include <snake.h>

static int zoom = 1;
static const int min_zoom = 1;
static const int max_zoom = 4;

#define CHAR_WIDTH (BASE_CHAR_WIDTH*zoom)       // Ancho de un char actual [EN PIXELES]
#define CHAR_HEIGHT (BASE_CHAR_HEIGHT*zoom)     // Alto de un char actual [EN PIXELES]

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)      // Ancho de pantalla [EN CHARS ACTUALES]
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)       // Alto de pantalla [EN CHARS ACTUALES]

#define COMMAND_LINE_X (2*BASE_CHAR_WIDTH)            // Pos de x de la linea de comandos [EN PIXELES]
#define COMMAND_LINE_Y (DIM_Y-(2*CHAR_HEIGHT))        // Pos de y de la linea de comandos [EN PIXELES]

#define COMMAND_DIM ((BASE_DIM_CHAR_X-4)*2)  // maximo tamaño de comando, sacando margenes [EN CHARS BASE]

static void inicializeShell();
static void getCommand();
static void doCommand();
static void cleanCommand();
static void printCommands();
static void cleanScreen();


static char exit = 0;
static int command_size = 0;
static char command[COMMAND_DIM];              // buffer de commando escrito
static char response[COMMAND_DIM];                 // buffer de respuesta de commando
static char commandDone = 0;                        // flag para checkear si un comando se realizo (no prints de mas)

static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int colorIndex = 0;
static int actualColor = white;
static int actualBackgroundFont=black;

static int fullLines = 0;


void getContextBack(){
    setZoom(zoom);
    setBackGroundColor(actualBackgroundFont);
    setFontColor(actualColor);
}

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
    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y);
    print("exited");
}

void getCommand() {
    char c;
    do {
        _hlt();
        c = getChar();
        if (c != '\0') {
            if (c != '\b') {
                command[command_size++] = c;
            } else {
                command_size -= (command_size > 0);  // le resto solo si es mayor a 0
                command[command_size] = ' ';
            }
            setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y);
            print(command);
        }
    } while (c != '\n' && command_size < (COMMAND_DIM-1)/zoom - 1);
    command[command_size-1] = '\0';
}

void doCommand() {
    if (command[0] != '\0' && command[0] != '\n') {
        commandDone = 1;
        if (strCaseCmp(command, "color") == 0) {
            actualColor=color[colorIndex];
            setFontColor(actualColor);
            colorIndex = (colorIndex+1)%21;
            strCpy("New color setted", response);
        } else if (strCaseCmp(command, "date")==0) {
            programTime(response);
            getContextBack();
        }else if (strCaseCmp(command, "rec")==0){
            programRectangle(actualColor);
            getContextBack();
            strCpy("Rectangle drawn", response);
        } else if (strCaseCmp(command, "help")==0) {
            // strCpy("Help", response);
            itoa(123, response, 10, 5);
        } else if (strCaseCmp(command, "zoom in") == 0) {
            if (zoom < max_zoom) { 
                cleanScreen();
                strCpy("Zoomed in", response);
                setZoom(++zoom);
                getContextBack();
            } else {
                strCpy("Max zoom possible", response);
            }
        } else if (strCaseCmp(command, "zoom out") == 0) {
            if (zoom > min_zoom) { 
                cleanScreen();
                strCpy("Zoomed out", response);
                setZoom(--zoom);
                getContextBack();
            } else {
                strCpy("Min zoom possible", response);
            }
        } else if (strCaseCmp(command, "snake")==0) {
            snake();
            getContextBack();
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
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
}

void printCommands() {

    drawRectangle((Point){1, COMMAND_LINE_Y-4*CHAR_HEIGHT}, (Point){DIM_X, COMMAND_LINE_Y+2*CHAR_HEIGHT}, 0x000000);
    
    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y-4*CHAR_HEIGHT);
    print(command);

    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y-2*CHAR_HEIGHT);
    print(response);

}


void inicializeShell() {
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
    setFontColor(white);
    print("> ");
    setFontColor(actualColor);
}

void cleanScreen() {
    drawRectangle((Point){1, (COMMAND_LINE_Y-4*zoom)*CHAR_HEIGHT}, (Point){DIM_X, DIM_X}, 0x000000);
}
