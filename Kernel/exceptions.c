#include <sysCallDispatcher.h>
#include <videoDriver.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6

extern void _hlt();
extern void _sti();
extern void _cli();
static void zero_division();
static void invalid_opcode();
static void printExceptionMsg(char * e);

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
    if (exception == INVALID_OPCODE)
        invalid_opcode();
}

void waitToReturn() {
    char c;
    do {
        _hlt();
        sys_read(STDIN, &c, 1);
    } while (c != '\n');
}

static void zero_division() {
    _sti();
    printExceptionMsg("Zero Division Error");
}

static void invalid_opcode() {
    _sti();
    printExceptionMsg("Invalid Opcode");
}

static void printExceptionMsg(char * e) {
    drawRectangle(&((Point){0,0}), &((Point){DIM_X, DIM_Y}), 0x000000);
    sys_setZoom(1);
    sys_setCursor(8, 16);
    sys_write(STDERR, "Exception: ", 11);
    sys_write(STDERR, e, strLen(e));
    sys_write(STDERR, "\n\n", 2);
    showRegisters();
    sys_sleep(20);
    waitToReturn();
    drawRectangle(&((Point){0,0}), &((Point){DIM_X, DIM_Y}), 0x000000);
}
