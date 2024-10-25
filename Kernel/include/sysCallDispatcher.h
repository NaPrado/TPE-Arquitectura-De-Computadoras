#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FDS;

typedef struct {
	int x;
	int y;
}Point;


void syscallDispatcher(uint64_t rax, ...);
void sys_write(FDS fd, const char *buf, size_t count);

#endif