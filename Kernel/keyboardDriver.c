#include <keyboardDriver.h>
#include <interrupts.h>

#define ESC 0x00
#define CAPSLOCK 0x3A
#define LSHIFT 0x36
#define RSHIFT 0x2A
#define LCTRL 0x1D
#define RCTRL 0x9D
#define LALT 0x38
#define RALT 0xB8
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
    [0x00] = -2, //Esc   
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
    [0x1D] = -2,    // left control no es imprimible
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
    [0x2A] = -2,    // left shift no es imprimible
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
    [0x36] = -2,    // right shift no es imprimible
    [0x37] = '*',   // (keypad) * es imprimible
    [0x38] = -2,    // left alt no es imprimible
    [0x39] = ' ',   
    [0x3A] = -2,    // CapsLock no es imprimible
    [0x3B] = -2,    // F1 no es imprimible
    [0x3C] = -2,    // F2 no es imprimible
    [0x3D] = -2,    // F3 no es imprimible
    [0x3E] = -2,    // F4 no es imprimible
    [0x3F] = -2,    // F5 no es imprimible
    [0x40] = -2,    // F6 no es imprimible
    [0x41] = -2,    // F7 no es imprimible
    [0x42] = -2,    // F8 no es imprimible
    [0x43] = -2,    // F9 no es imprimible
    [0x44] = -2,    // F10 no es imprimible
    [0x45] = -2,    // NumLock no es imprimible
    [0x46] = -2,    // ScrollLock no es imprimible
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
    [0x54] = -2,    // (keypad) Enter no es imprimible
    [0x55] = '/',   // (keypad) / es imprimible
    [0x56] = -2,    // Right Alt no es imprimible
    [0x57] = -2,    // Home no es imprimible
    [0x58] = -2,    // Up Arrow no es imprimible
    [0x59] = -2,    // Page Up no es imprimible
    [0x5A] = -2,    // Left Arrow no es imprimible
    [0x5B] = -2,    // Right Arrow no es imprimible
    [0x5C] = -2,    // End no es imprimible
    [0x5D] = -2,    // Down Arrow no es imprimible
    [0x5E] = -2,    // Page Down no es imprimible
    [0x5F] = -2,    // Insert no es imprimible
    [0x60] = -2,    // Delete no es imprimible
    [0x61] = -2,    // F11 no es imprimible
    [0x62] = -2,    // F12 no es imprimible
    [0x63] = -2,    // F13 no es imprimible
    [0x64] = -2,    // F14 no es imprimible
    [0x65] = -2,    // F15 no es imprimible
    [0x66] = -2,    // F16 no es imprimible
    [0x67] = -2,    // F17 no es imprimible
    [0x68] = -2,    // F18 no es imprimible
    [0x69] = -2,    // F19 no es imprimible
    [0x6A] = -2,    // F20 no es imprimible
    [0x6B] = -2,    // F21 no es imprimible
    [0x6C] = -2,    // F22 no es imprimible
    [0x6D] = -2,    // F23 no es imprimible
    [0x6E] = -2,    // F24 no es imprimible
    [0x6F] = -2,    // (keypad) Enter no es imprimible
    [0x70] = -2,    // (keypad) = no es imprimible
    [0x71] = -2,    // (keypad) , no es imprimible
    [0x72] = -2,    // (keypad) ( no es imprimible
    [0x73] = -2,    // (keypad) ) no es imprimible
    [0x74] = -2,    // (keypad) $ no es imprimible
    [0x75] = -2,    // (keypad) # no es imprimible
    [0x76] = -2,    // (keypad) @ no es imprimible
    [0x77] = -2,    // (keypad) ! no es imprimible
    [0x78] = -2,    // (keypad) % no es imprimible
    [0x79] = -2,    // (keypad) ^ no es imprimible
    [0x7A] = -2,    // (keypad) & no es imprimible
    [0x7B] = -2,    // (keypad) * no es imprimible
    [0x7C] = -2,    // (keypad) - no es imprimible
    [0x7D] = -2,    // (keypad) + no es imprimible
    [0x7E] = -2,    // (keypad) / no es imprimible
    [0x7F] = -2     // (keypad) . no es imprimible
    };

static int shiftedAscii[] = {
        [0x01] = -2,   //Esc
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
        [0x1D] = -2,    // left control no es imprimible
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
        [0x2A] = -2,    // left shift no es imprimible
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
        [0x36] = -2,    // right shift no es imprimible
        [0x37] = '*',   // (keypad) * es imprimible
        [0x38] = -2,    // left alt no es imprimible
        [0x39] = ' ',   
        [0x3A] = -2,    // CapsLock no es imprimible
        [0x3B] = -2,    // F1 no es imprimible
        [0x3C] = -2,    // F2 no es imprimible
        [0x3D] = -2,    // F3 no es imprimible
        [0x3E] = -2,    // F4 no es imprimible
        [0x3F] = -2,    // F5 no es imprimible
        [0x40] = -2,    // F6 no es imprimible
        [0x41] = -2,    // F7 no es imprimible
        [0x42] = -2,    // F8 no es imprimible
        [0x43] = -2,    // F9 no es imprimible
        [0x44] = -2,    // F10 no es imprimible
        [0x45] = -2,    // NumLock no es imprimible
        [0x46] = -2,    // ScrollLock no es imprimible
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
        [0x54] = -2,    // (keypad) Enter no es imprimible
        [0x55] = '/',   // (keypad) / es imprimible
        [0x56] = -2,    // Right Alt no es imprimible
        [0x57] = -2,    // Home no es imprimible
        [0x58] = -2,    // Up Arrow no es imprimible
        [0x59] = -2,    // Page Up no es imprimible
        [0x5A] = -2,    // Left Arrow no es imprimible
        [0x5B] = -2,    // Right Arrow no es imprimible
        [0x5C] = -2,    // End no es imprimible
        [0x5D] = -2,    // Down Arrow no es imprimible
        [0x5E] = -2,    // Page Down no es imprimible
        [0x5F] = -2,    // Insert no es imprimible
        [0x60] = -2,    // Delete no es imprimible
        [0x61] = -2,    // F11 no es imprimible
        [0x62] = -2,    // F12 no es imprimible
        [0x63] = -2,    // F13 no es imprimible
        [0x64] = -2,    // F14 no es imprimible
        [0x65] = -2,    // F15 no es imprimible
        [0x66] = -2,    // F16 no es imprimible
        [0x67] = -2,    // F17 no es imprimible
        [0x68] = -2,    // F18 no es imprimible
        [0x69] = -2,    // F19 no es imprimible
        [0x6A] = -2,    // F20 no es imprimible
        [0x6B] = -2,    // F21 no es imprimible
        [0x6C] = -2,    // F22 no es imprimible
        [0x6D] = -2,    // F23 no es imprimible
        [0x6E] = -2,    // F24 no es imprimible
        [0x6F] = -2,    // (keypad) Enter no es imprimible
        [0x70] = -2,    // (keypad) = no es imprimible
        [0x71] = -2,    // (keypad) , no es imprimible
        [0x72] = -2,    // (keypad) ( no es imprimible
        [0x73] = -2,    // (keypad) ) no es imprimible
        [0x74] = -2,    // (keypad) $ no es imprimible
        [0x75] = -2,    // (keypad) # no es imprimible
        [0x76] = -2,    // (keypad) @ no es imprimible
        [0x77] = -2,    // (keypad) ! no es imprimible
        [0x78] = -2,    // (keypad) % no es imprimible
        [0x79] = -2,    // (keypad) ^ no es imprimible
        [0x7A] = -2,    // (keypad) & no es imprimible
        [0x7B] = -2,    // (keypad) * no es imprimible
        [0x7C] = -2,    // (keypad) - no es imprimible
        [0x7D] = -2,    // (keypad) + no es imprimible
        [0x7E] = -2,    // (keypad) / no es imprimible
        [0x7F] = -2     // (keypad) . no es imprimible
    };

char hasNextKey() {
    return charsAtBuffer > 0;
}

int nextKey() {
    int ret;
    if (!hasNextKey()) {
        return -2;
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
    if (scancode == ESC) {
        return;
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