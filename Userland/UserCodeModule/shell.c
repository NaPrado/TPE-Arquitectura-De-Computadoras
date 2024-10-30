#include <shell.h>
#include <libc.h>
#include <stdint.h>
#include <snake.h>
#include <libasm.h>

int zoom_user = 1;
const int min_zoom = 1;
const int max_zoom = 4;

#define CHAR_WIDTH (BASE_CHAR_WIDTH*zoom_user)       // Ancho de un char actual [EN PIXELES]
#define CHAR_HEIGHT (BASE_CHAR_HEIGHT*zoom_user)     // Alto de un char actual [EN PIXELES]

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
    setZoom(zoom_user);
    setBackGroundColor(actualBackgroundFont);
    setFontColor(actualColor);
}

void shell() {
    setCursor(COMMAND_LINE_X+2*CHAR_WIDTH, COMMAND_LINE_Y-4*CHAR_HEIGHT);
    print("Welcome to the shell\n\n\tType 'help' to see the available commands");
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
    } while (c != '\n' && command_size < (COMMAND_DIM-1)/zoom_user - 1);
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
            cleanScreen();
            programRectangle(actualColor);
            getContextBack();
            strCpy("Rectangle exited", response);
        } else if (strCaseCmp(command, "div0")==0) {
            _div();
        } else if (strCaseCmp(command, "invOp")==0) {
            _ioe();
        } else if (strCaseCmp(command, "help")==0) {
            cleanScreen();
            programHelp();
            getContextBack();
            strCpy("Help exited", response);
        } else if (strCaseCmp(command, "zoom in") == 0) {
            if (zoom_user < max_zoom) { 
                cleanScreen();
                strCpy("Zoomed in", response);
                setZoom(++zoom_user);
                getContextBack();
            } else {
                strCpy("Max zoom possible", response);
            }
        } else if (strCaseCmp(command, "zoom out") == 0) {
            if (zoom_user > min_zoom) { 
                cleanScreen();
                strCpy("Zoomed out", response);
                setZoom(--zoom_user);
                getContextBack();
            } else {
                strCpy("Min zoom possible", response);
            }
        } else if (strCaseCmp(command, "snake")==0) {
            cleanScreen();
            snake();
            getContextBack();
            strCpy("Snake exited", response);
        } else if (strCaseCmp(command, "registers")==0) {
            cleanScreen();
            programRegisters();
            getContextBack();
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
    drawRectangle((Point){1, COMMAND_LINE_Y-4*CHAR_HEIGHT}, (Point){DIM_X, DIM_Y}, 0x000000);
}
