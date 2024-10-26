#include <shell.h>
#include <libc.h>
#include <stdint.h>

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)

#define COMMAND_LINE_X 4                        // Pos de x de la linea de comandos
#define COMMAND_LINE_Y (DIM_CHAR_Y-4)           // Pos de y de la linea de comandos

static void inicializeShell();
static void getCommand();
static void doCommand();
static void cleanCommand();
static void printCommands();
static void cleanScreen();

static char exit = 0;
static const int command_dim = DIM_CHAR_X*2;
static int command_size = 0;
static char command[(DIM_CHAR_X-4)*2];

static int buffer_command_start = 0;
static int buffer_command_size = 0;
static char buffer_command[2][DIM_CHAR_X*2];


static uint32_t pos_x = COMMAND_LINE_X;
static uint32_t pos_y = COMMAND_LINE_Y;

static int zoom = 1;


void shell() {
    inicializeShell();

    while (!exit) {
        printCommands();
        cleanCommand();
        getCommand();
        doCommand();
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
    } while (c != '\n' && command_size < command_dim-1);
    command[command_size-1] = 0;
}

static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int color_index = 0;

static int fullLines = 0;

void doCommand() {
    if (command[0] != 0 && command[0] != '\n') {
        // if (buffer_command_size == (DIM_CHAR_Y-4)/2){
        //     fullLines=1;
        //     buffer_command_size = 0;
        //     buffer_command_start++;
        // }
        // strCpy(command, buffer_command[buffer_command_size++]);

        strNCpy(command, buffer_command[0], command_dim);
        if (strCaseCmp(command, "color") == 0) {
            setFontColor(color[color_index]);
            color_index = (color_index+1)%21;
            strCpy("New color setted", buffer_command[1]);
        } else if (strCaseCmp(command, "date")==0) {
            char * time = getTime();
            // strCpy(time, buffer_command[buffer_command_size++]);
            strCpy(time, buffer_command[1]);
        }else if (strCaseCmp(command, "rec")==0){
            Point p1 = {100, 100};
            Point p2 = {200, 200};
            drawRectangle(p1, p2, 0x00FF00);
            strCpy("Rectangle drawn", buffer_command[1]);
        } else if (strCaseCmp(command, "help")==0) {
            strCpy("Help", buffer_command[1]);
        } else if (strCaseCmp(command, "zoom in") == 0) {
            if (zoom <= 3) { 
                strCpy("Zoomed in", buffer_command[1]);
                setZoom(++zoom);
            } else {
                strCpy("Max zoom possible", buffer_command[1]);
            }
        } else if (strCaseCmp(command, "zoom out") == 0) {
            if (zoom > 1) { 
                strCpy("Zoomed out", buffer_command[1]);
                setZoom(--zoom);
            } else {
                strCpy("Min zoom possible", buffer_command[1]);
            }
        } else if (strCaseCmp(command, "exit")==0) {
            strCpy("Exit", buffer_command[1]);
            cleanScreen();
            exit = 1;
        } else {
            // strCpy("Command not found", buffer_command[buffer_command_size++]);
            strCpy("Command not found", buffer_command[1]);
        }
    }
}

void cleanCommand() {
    for (int i = 0; i < command_size; i++) {
        command[i] = '\0';
    }
    command_size = 0;
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y);
    nprint(command, command_dim);
}

void printCommands() {
    // int i = buffer_command_start;
    // int count = 0;
    // int toPrint = fullLines?21:buffer_command_size;
    // for ( ; i < toPrint; i++) {
    //     setCursor(COMMAND_LINE_X, COMMAND_LINE_Y - (toPrint-i)*2);
    //     print(buffer_command[i]);
    //     char clean[120]={0};
    //     printByLenght(clean,120-strlen(buffer_command[i])); //limpia la linea, usar defines
    // }

    char clean[DIM_CHAR_X*4]={0x00};    //limpia la linea, usar defines
    for (int i = 1; i <= zoom; i++) {
        setCursor(0, COMMAND_LINE_Y-4*i);
        nprint(clean, DIM_CHAR_X*4); 
    }

    
    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y-4*zoom);
    print(buffer_command[0]);

    setCursor(COMMAND_LINE_X, COMMAND_LINE_Y-2*zoom);
    print(buffer_command[1]);

}


void inicializeShell() {
    setCursor(COMMAND_LINE_X-2, COMMAND_LINE_Y);
    setFontColor(white);
    print("> ");
}

void cleanScreen() {
    char cleanScreen[DIM_CHAR_X*DIM_CHAR_Y] = {0x00};
    setCursor(0,0);
    nprint(cleanScreen, DIM_CHAR_X*DIM_CHAR_Y);
}
