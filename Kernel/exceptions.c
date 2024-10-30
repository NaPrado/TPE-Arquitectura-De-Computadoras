#include <sysCallDispatcher.h>
#define ZERO_EXCEPTION_ID 0

extern void loader();
static void zero_division();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
}

static void zero_division() {
	showRegisters();
}
