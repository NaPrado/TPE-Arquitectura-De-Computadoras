#include "include/libc.h"

extern uint64_t sys_call(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t r10, uint64_t r8);

void printf(const char * buf) {
    sys_call(1, 1, (uint64_t) buf, (uint64_t) strlen(buf), 0);
}

void scanf(const char * buf, uint32_t count) {
    sys_call(0, 0, (uint64_t) buf, (uint64_t) count, 0);
}

int itoa(uint64_t value, char * buffer) {
    char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % 10;
		*p++ = remainder + '0';
		digits++;
	} while (value /= 10);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

char getChar() {
    char buf[1] = {0};
    scanf(buf, 1);
    return buf[0];
}

void putChar(char c) {
    char buf[2] = {c, '\0'};
    printf(buf);
}

int strlen(const char * str) {
    int len = 0;
    while (*str != '\0') {
        len++;
    }
    return len;
}

void setCursor(uint32_t x, uint32_t y) {
    sys_call((uint64_t) 5, (uint64_t) x, (uint64_t) y, 0, 0);
}
