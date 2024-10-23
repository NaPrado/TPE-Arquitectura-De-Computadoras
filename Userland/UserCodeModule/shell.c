#include "./include/shell.h"
#include <stdint.h>

static void inicializeShell();

#define COMMAND_X 4
#define COMMAND_Y ((DIM_Y/CHAR_HEIGHT)-4)

char exit = 0;
char * current_comand[128 * 2] = {0x00};


// void shell() {}
//     inicializeShell();

//     uint32_t pos_x = COMMAND_X;
//     uint32_t pos_y = COMMAND_Y;

//     while (!exit) {
//         if (hasNextKey()){	
// 			char caps = (getShiftPressed()^getCapslock());
// 			char key = nextKey();
// 			char c = keyToAscii(key);
// 			char isLowercase=(c>='a' && c<='z');
// 			if (key==0x1C){// Enter
// 			}
// 			if (key==0x0E){// Backspace

// 			} else {
//                 putchar(c - (caps&&isLowercase ? ('a'-'A'):0), pos_x, pos_y);
//                 pos_y += (pos_x%(DIM_X/CHAR_WIDTH) == 0);
//                 pos_x = (pos_x%(DIM_X/CHAR_WIDTH) == 0) ? 0 : pos_x+1;
// 			}
// 		}

//         redrawScreen();
		
// 	}
// }
    

// void inicializeShell() {
//     putchar('>', 2, (DIM_Y/CHAR_HEIGHT) - 4);
// }
