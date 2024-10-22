/***************************************************
  libc.h
****************************************************/

#include <stdint.h>

#ifndef _LIBC_H_
#define _LIBC_H_

void printf(const char * buf);

void scanf(const char * buf, uint32_t count);

void itoa(uint64_t value, char * buffer);

char getChar();

void putChar(char c);

int strlen(const char * str);


#endif