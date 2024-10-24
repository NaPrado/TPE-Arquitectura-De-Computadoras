/***************************************************
  libc.h
****************************************************/

#include <stdint.h>

#ifndef _LIBC_H_
#define _LIBC_H_

void print(char * buf);

void scan(char * buf, uint32_t count);

int itos(uint64_t value, char * buffer, int base);

char getChar();

void putChar(char c);

int strlen(const char * str);

void setCursor(uint32_t x, uint32_t y);

void sleep(uint64_t seconds);

void strCpy(char * s, char * t);

int strCmp(char * s1, char * s2);


#endif