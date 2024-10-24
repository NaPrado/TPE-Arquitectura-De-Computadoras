#include <shell.h>
#include <libc.h>
#include <stdint.h>

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)

// static void inicializeShell();

#define COMMAND_X 4
#define COMMAND_Y (DIM_CHAR_Y-4)

static void getCommand();
static void doCommand();
static void cleanCommand();
static void printCommands();

static char exit = 0;
static const int command_dim = DIM_CHAR_X*2;
static int command_size = 0;
static char command[(DIM_CHAR_X-4)*2];

static int buffer_command_size = 0;
static char buffer_command[(DIM_CHAR_X-4)*2][(DIM_CHAR_Y-4)/2];


static uint32_t pos_x = COMMAND_X;
static uint32_t pos_y = COMMAND_Y;


void shell() {
    // inicializeShell();

    setCursor(pos_x-2, pos_y);
    print("> ");

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
    } while (c != '\n' && command_size < command_dim);
}

void doCommand() {
    if (command[0] != ' ' && command[0] != '\n') {
        command[command_size-1] = '\0';
        strCpy(command, buffer_command[buffer_command_size++]);
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
    for (int i = 0; i < buffer_command_size; i++) {
        setCursor(COMMAND_X, COMMAND_Y - (buffer_command_size-i)*2);
        print(buffer_command[i]);
    }
}
    

// void inicializeShell() {
//     putchar('>', 2, (DIM_Y/CHAR_HEIGHT) - 4);
// }
