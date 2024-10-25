/***************************************************
  libc.h
****************************************************/

#include <stdint.h>

#ifndef _LIBC_H_
#define _LIBC_H_


typedef struct {
	int x;
	int y;
}Point;

void drawRectangle(Point topLeft, Point downRigth, uint32_t color) ;

void setFontColor(uint32_t hexColor);

void print(char * buf);

void nprint(char * buf, uint64_t lenght);

void scan(char * buf, uint32_t count);

int itos(uint64_t value, char * buffer, int base);

char getChar();

void putChar(char c);

int strlen(const char * str);

void setCursor(uint32_t x, uint32_t y);

void sleep(uint64_t seconds);

void strCpy(char * source, char * dest);

void strNCpy(char * source, char * dest, int n);

int strCmp(const char * s1,const char * s2);

int strCaseCmp(const char * s1, const char * s2);

int isalpha(int c);

int toupper(int c);

int tolower(int c);

// int strStartsWith(const char * str, const char * start);

enum colors{
    black = 0x000000,
    white = 0xFFFFFF,
    blue = 0x0000FF,
    green = 0x00FF00,
    red = 0xFF0000,
    yellow = 0xFFFF00,
    purple = 0xFF00FF,
    cyan = 0x00FFFF,
    orange = 0xFFA500,
    pink = 0xFFC0CB,
    brown = 0xA52A2A,
    lightGrey = 0xD3D3D3,
    lightBlue = 0xADD8E6,
    lightGreen = 0x90EE90,
    lightRed = 0xFFA07A,
    lightPink = 0xFFB6C1,
    lightBrown = 0xD2B48C,
    darkBlue = 0x00008B,
    darkGreen = 0x006400,
    darkRed = 0x8B0000,
    darkYellow = 0x9B870C,
    darkPurple = 0x800080,
    darkCyan = 0x008B8B,
    darkOrange = 0xFF8C00,
    darkPink = 0xFF1493,
    darkBrown = 0x654321
};


#endif