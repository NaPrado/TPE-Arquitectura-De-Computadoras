#include <keyboardDriver.h>
#include <interrupts.h>

#define ENTER 0x01
#define CAPSLOCK 0x3A
#define LSHIFT 0x36
#define RSHIFT 0x2A
#define LCTRL 0x1D
#define RCTRL 0x9D
#define LALT 0x38
#define RALT 0xB8
#define ESC 0x01
#define CHAR_BUFFER_DIM 64

static char charBuffer[CHAR_BUFFER_DIM] = {0};
static char charsAtBuffer=0;
static int charBufferIndex=0;
static char getterIndex=0;
static char capslock=0;
static char shift=0;
static char ctrl=0;
static char alt=0;

extern void saveRegisters();

static int notShiftedAscii[] = {
    [0x01] = -1, //Esc   
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = '\b',    // backspace no es imprimible
    [0x0F] = '\t',    // tab no es imprimible
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n',    // enter no es imprimible
    [0x1D] = -1,    // left control no es imprimible
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    [0x2A] = -1,    // left shift no es imprimible
    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x36] = -1,    // right shift no es imprimible
    [0x37] = '*',   // (keypad) * es imprimible
    [0x38] = -1,    // left alt no es imprimible
    [0x39] = ' ',   
    [0x3A] = -1,    // CapsLock no es imprimible
    [0x3B] = -1,    // F1 no es imprimible
    [0x3C] = -1,    // F2 no es imprimible
    [0x3D] = -1,    // F3 no es imprimible
    [0x3E] = -1,    // F4 no es imprimible
    [0x3F] = -1,    // F5 no es imprimible
    [0x40] = -1,    // F6 no es imprimible
    [0x41] = -1,    // F7 no es imprimible
    [0x42] = -1,    // F8 no es imprimible
    [0x43] = -1,    // F9 no es imprimible
    [0x44] = -1,    // F10 no es imprimible
    [0x45] = -1,    // NumLock no es imprimible
    [0x46] = -1,    // ScrollLock no es imprimible
    [0x47] = '7',   // (keypad) 7 
    [0x48] = '8',   // (keypad) 8 
    [0x49] = '9',   // (keypad) 9 
    [0x4A] = '-',   // (keypad) - 
    [0x4B] = '4',   // (keypad) 4 
    [0x4C] = '5',   // (keypad) 5 
    [0x4D] = '6',   // (keypad) 6 
    [0x4E] = '+',   // (keypad) + 
    [0x4F] = '1',   // (keypad) 1 
    [0x50] = '2',   // (keypad) 2 
    [0x51] = '3',   // (keypad) 3 
    [0x52] = '0',   // (keypad) 0 
    [0x53] = '.',   // (keypad) . 
    [0x54] = -1,    // (keypad) Enter no es imprimible
    [0x55] = '/',   // (keypad) / es imprimible
    [0x56] = -1,    // Right Alt no es imprimible
    [0x57] = -1,    // Home no es imprimible
    [0x58] = -1,    // Up Arrow no es imprimible
    [0x59] = -1,    // Page Up no es imprimible
    [0x5A] = -1,    // Left Arrow no es imprimible
    [0x5B] = -1,    // Right Arrow no es imprimible
    [0x5C] = -1,    // End no es imprimible
    [0x5D] = -1,    // Down Arrow no es imprimible
    [0x5E] = -1,    // Page Down no es imprimible
    [0x5F] = -1,    // Insert no es imprimible
    [0x60] = -1,    // Delete no es imprimible
    [0x61] = -1,    // F11 no es imprimible
    [0x62] = -1,    // F12 no es imprimible
    [0x63] = -1,    // F13 no es imprimible
    [0x64] = -1,    // F14 no es imprimible
    [0x65] = -1,    // F15 no es imprimible
    [0x66] = -1,    // F16 no es imprimible
    [0x67] = -1,    // F17 no es imprimible
    [0x68] = -1,    // F18 no es imprimible
    [0x69] = -1,    // F19 no es imprimible
    [0x6A] = -1,    // F20 no es imprimible
    [0x6B] = -1,    // F21 no es imprimible
    [0x6C] = -1,    // F22 no es imprimible
    [0x6D] = -1,    // F23 no es imprimible
    [0x6E] = -1,    // F24 no es imprimible
    [0x6F] = -1,    // (keypad) Enter no es imprimible
    [0x70] = -1,    // (keypad) = no es imprimible
    [0x71] = -1,    // (keypad) , no es imprimible
    [0x72] = -1,    // (keypad) ( no es imprimible
    [0x73] = -1,    // (keypad) ) no es imprimible
    [0x74] = -1,    // (keypad) $ no es imprimible
    [0x75] = -1,    // (keypad) # no es imprimible
    [0x76] = -1,    // (keypad) @ no es imprimible
    [0x77] = -1,    // (keypad) ! no es imprimible
    [0x78] = -1,    // (keypad) % no es imprimible
    [0x79] = -1,    // (keypad) ^ no es imprimible
    [0x7A] = -1,    // (keypad) & no es imprimible
    [0x7B] = -1,    // (keypad) * no es imprimible
    [0x7C] = -1,    // (keypad) - no es imprimible
    [0x7D] = -1,    // (keypad) + no es imprimible
    [0x7E] = -1,    // (keypad) / no es imprimible
    [0x7F] = -1     // (keypad) . no es imprimible
    };

static int shiftedAscii[] = {
        [0x01] = -1,   //Esc
        [0x02] = '!',
        [0x03] = '@',
        [0x04] = '#',
        [0x05] = '$',
        [0x06] = '%',
        [0x07] = '^',
        [0x08] = '&',
        [0x09] = '*',
        [0x0A] = '(',
        [0x0B] = ')',
        [0x0C] = '_',
        [0x0D] = '+',
        [0x0E] = '\b',    // backspace no es imprimible
        [0x0F] = '\t',    // tab no es imprimible
        [0x10] = 'Q',
        [0x11] = 'W',
        [0x12] = 'E',
        [0x13] = 'R',
        [0x14] = 'T',
        [0x15] = 'Y',
        [0x16] = 'U',
        [0x17] = 'I',
        [0x18] = 'O',
        [0x19] = 'P',
        [0x1A] = '{',
        [0x1B] = '}',
        [0x1C] = '\n',    // enter no es imprimible
        [0x1D] = -1,    // left control no es imprimible
        [0x1E] = 'A',
        [0x1F] = 'S',
        [0x20] = 'D',
        [0x21] = 'F',
        [0x22] = 'G',
        [0x23] = 'H',
        [0x24] = 'J',
        [0x25] = 'K',
        [0x26] = 'L',
        [0x27] = ':',
        [0x28] = '"',
        [0x29] = '~',
        [0x2A] = -1,    // left shift no es imprimible
        [0x2B] = '|',
        [0x2C] = 'Z',
        [0x2D] = 'X',
        [0x2E] = 'C',
        [0x2F] = 'V',
        [0x30] = 'B',
        [0x31] = 'N',
        [0x32] = 'M',
        [0x33] = '<',
        [0x34] = '>',
        [0x35] = '?',
        [0x36] = -1,    // right shift no es imprimible
        [0x37] = '*',   // (keypad) * es imprimible
        [0x38] = -1,    // left alt no es imprimible
        [0x39] = ' ',   
        [0x3A] = -1,    // CapsLock no es imprimible
        [0x3B] = -1,    // F1 no es imprimible
        [0x3C] = -1,    // F2 no es imprimible
        [0x3D] = -1,    // F3 no es imprimible
        [0x3E] = -1,    // F4 no es imprimible
        [0x3F] = -1,    // F5 no es imprimible
        [0x40] = -1,    // F6 no es imprimible
        [0x41] = -1,    // F7 no es imprimible
        [0x42] = -1,    // F8 no es imprimible
        [0x43] = -1,    // F9 no es imprimible
        [0x44] = -1,    // F10 no es imprimible
        [0x45] = -1,    // NumLock no es imprimible
        [0x46] = -1,    // ScrollLock no es imprimible
        [0x47] = '7',   // (keypad) 7 
        [0x48] = '8',   // (keypad) 8 
        [0x49] = '9',   // (keypad) 9 
        [0x4A] = '-',   // (keypad) - 
        [0x4B] = '4',   // (keypad) 4 
        [0x4C] = '5',   // (keypad) 5 
        [0x4D] = '6',   // (keypad) 6 
        [0x4E] = '+',   // (keypad) + 
        [0x4F] = '1',   // (keypad) 1 
        [0x50] = '2',   // (keypad) 2 
        [0x51] = '3',   // (keypad) 3 
        [0x52] = '0',   // (keypad) 0 
        [0x53] = '.',   // (keypad) . 
        [0x54] = -1,    // (keypad) Enter no es imprimible
        [0x55] = '/',   // (keypad) / es imprimible
        [0x56] = -1,    // Right Alt no es imprimible
        [0x57] = -1,    // Home no es imprimible
        [0x58] = -1,    // Up Arrow no es imprimible
        [0x59] = -1,    // Page Up no es imprimible
        [0x5A] = -1,    // Left Arrow no es imprimible
        [0x5B] = -1,    // Right Arrow no es imprimible
        [0x5C] = -1,    // End no es imprimible
        [0x5D] = -1,    // Down Arrow no es imprimible
        [0x5E] = -1,    // Page Down no es imprimible
        [0x5F] = -1,    // Insert no es imprimible
        [0x60] = -1,    // Delete no es imprimible
        [0x61] = -1,    // F11 no es imprimible
        [0x62] = -1,    // F12 no es imprimible
        [0x63] = -1,    // F13 no es imprimible
        [0x64] = -1,    // F14 no es imprimible
        [0x65] = -1,    // F15 no es imprimible
        [0x66] = -1,    // F16 no es imprimible
        [0x67] = -1,    // F17 no es imprimible
        [0x68] = -1,    // F18 no es imprimible
        [0x69] = -1,    // F19 no es imprimible
        [0x6A] = -1,    // F20 no es imprimible
        [0x6B] = -1,    // F21 no es imprimible
        [0x6C] = -1,    // F22 no es imprimible
        [0x6D] = -1,    // F23 no es imprimible
        [0x6E] = -1,    // F24 no es imprimible
        [0x6F] = -1,    // (keypad) Enter no es imprimible
        [0x70] = -1,    // (keypad) = no es imprimible
        [0x71] = -1,    // (keypad) , no es imprimible
        [0x72] = -1,    // (keypad) ( no es imprimible
        [0x73] = -1,    // (keypad) ) no es imprimible
        [0x74] = -1,    // (keypad) $ no es imprimible
        [0x75] = -1,    // (keypad) # no es imprimible
        [0x76] = -1,    // (keypad) @ no es imprimible
        [0x77] = -1,    // (keypad) ! no es imprimible
        [0x78] = -1,    // (keypad) % no es imprimible
        [0x79] = -1,    // (keypad) ^ no es imprimible
        [0x7A] = -1,    // (keypad) & no es imprimible
        [0x7B] = -1,    // (keypad) * no es imprimible
        [0x7C] = -1,    // (keypad) - no es imprimible
        [0x7D] = -1,    // (keypad) + no es imprimible
        [0x7E] = -1,    // (keypad) / no es imprimible
        [0x7F] = -1     // (keypad) . no es imprimible
    };

char hasNextKey() {
    return charsAtBuffer > 0;
}

// waitKey() {
//     while (!hasNextKey()) {
//         picMasterMask(PIC_MASTER_MASK_ONLY_KEYBOARD);
//         _hlt();
//         picMasterMask(PIC_MASTER_MASK_ALL); 
//     }
// }

int nextKey() {
    int ret;
    if (!hasNextKey()) {
        return -2;
        // waitKey();
    }
    charsAtBuffer--;
    ret = charBuffer[getterIndex];
    getterIndex++;
    getterIndex = getterIndex % CHAR_BUFFER_DIM;
    return ret;
}

char isSpecialKey(char scancode) {
    return 
    (scancode == LSHIFT)    || (scancode == RSHIFT)     || 
    (scancode == LCTRL)     || (scancode == RCTRL)      || 
    (scancode == LALT)      || (scancode == RALT)       ||
    (scancode == CAPSLOCK)  || (scancode == ESC)        ||
    (scancode == 0x57)      || (scancode == 0x58)       ||
    ((scancode >= 0x3B) && (scancode <= 0x44));
}

void keyboard_handler() {
    char scancode = getKey();
    char release = scancode;
    release = release >> 7;
    char key = scancode & 0x7F;
    if (scancode == ENTER) {
        saveRegisters();
    }
    if (scancode == CAPSLOCK) {
        capslock =! capslock;
        return;
    } else if ((key == LSHIFT || key == RSHIFT)) {
        shift =! release;
        return;
    } else if (key == LCTRL || key == RCTRL) {
        ctrl=!release;
        return;
    } else if (key == LALT || key == RALT) {
        alt =! release;
        return;
    } else {
        if (charsAtBuffer <= CHAR_BUFFER_DIM && !isSpecialKey(scancode) && !release){
            charBuffer[charBufferIndex++] = (shift^capslock) ? shiftedAscii[key] : notShiftedAscii[key];
            charBufferIndex = charBufferIndex % CHAR_BUFFER_DIM;
            charsAtBuffer++;
        }
    }
}