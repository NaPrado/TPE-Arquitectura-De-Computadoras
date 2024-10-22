#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FDS;


void syscallDispatcher(uint64_t rax, ...);
size_t sys_write(FDS fd, const char *buf, size_t count);

#endif