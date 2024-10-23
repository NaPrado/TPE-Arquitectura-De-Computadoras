/***************************************************
  libc.h
****************************************************/

#include <stdint.h>

#ifndef _LIBC_H_
#define _LIBC_H_

void print(char * buf);

void scan(const char * buf, uint32_t count);

int itos(uint64_t value, char * buffer, int base);

char getChar();

void putChar(char c);

int strlen(const char * str);

void setCursor(uint32_t x, uint32_t y);


#endif