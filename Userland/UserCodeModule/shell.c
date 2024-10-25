#include <shell.h>
#include <libc.h>
#include <stdint.h>

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)

#define COMMAND_X 4
#define COMMAND_Y (DIM_CHAR_Y-4)

static void inicializeShell();
static void getCommand();
static void doCommand();
static void cleanCommand();
static void printCommands();

static char exit = 0;
static const int command_dim = 120;//DIM_CHAR_X*2;
static int command_size = 0;
static char command[(DIM_CHAR_X-4)*2];

static int buffer_command_start = 0;
static int buffer_command_size = 0;
static char buffer_command[(DIM_CHAR_X-4)*2][(DIM_CHAR_Y-4)/2];


static uint32_t pos_x = COMMAND_X;
static uint32_t pos_y = COMMAND_Y;


void shell() {
    inicializeShell();

    while (!exit) {
        cleanCommand();
        getCommand();
        doCommand();
        printCommands();
	}
}

void getCommand() {
    char c;
    do {
        c = getChar();
        command[command_size++] = c;
        setCursor(COMMAND_X, COMMAND_Y);
        print(command);
    } while (c != '\n' && command_size < command_dim-1);
    command[command_size-1] = 0;
}
static uint32_t color[] = { blue, green, red, yellow, purple, cyan, orange, pink, brown, lightGrey, lightBlue, lightGreen, lightRed, lightPink, lightBrown, darkBlue, darkGreen, darkRed, darkYellow, darkPurple,white};
static int color_index = 0;

static int fullLines = 0;

void doCommand() {
    if (command[0] != 0 && command[0] != '\n') {
        if (buffer_command_size == 21){//(DIM_CHAR_Y-4)/2) {
            fullLines=1;
            buffer_command_size = 0;
            buffer_command_start++;
        }
        strCpy(command, buffer_command[buffer_command_size++]);
        if (strCmp(command, "color") == 0) {
            setFontColor(color[color_index]);
            color_index = (color_index+1)%26;
        }else if (strCmp(command, "date")==0){
            char * time = getTime();
            strCpy(time, buffer_command[buffer_command_size++]);
        }else{
            strCpy("Command not found", buffer_command[buffer_command_size++]);
        }
    }
    
}

void cleanCommand() {
    for (int i = 0; i < command_size; i++) {
        command[i] = ' ';
    }
    command[command_size-1] = '\0';
    command_size = 0;
    setCursor(COMMAND_X, COMMAND_Y);
    print(command);
}

void printCommands() {
    int i = buffer_command_start;
    int count = 0;
    int toPrint = fullLines?21:buffer_command_size;
    for ( ; i < toPrint; i++) {
        setCursor(COMMAND_X, COMMAND_Y - (toPrint-i)*2);
        print(buffer_command[i]);
        char clean[120]={0};
        printByLenght(clean,120-strlen(buffer_command[i]));//limpia la linea, usar defines
    }
    //int j = 0;
    // for ( ; j < i; j++) {
    //     setCursor(COMMAND_X, COMMAND_Y - (i-j)*2);
    //     print(buffer_command[j]);
    // }
    /*QUE HACE ESTO WTF*/
}
    

void inicializeShell() {
    setCursor(COMMAND_X-2, COMMAND_Y);
    setFontColor(white);
    print("> ");
}
