#include <sysCallDispatcher.h>
#include <stdarg.h>
#include <videoDriver.h>
#include <time.h>
#include <videoDriver.h>

static void writeFiles(FDS fd, const char *buf, size_t count);
static size_t sys_setCursor(int x, int y);
static void setFontColor(uint32_t hexColor);
static void sys_write(FDS fd, const char *buf, size_t count);
static void readFiles(fd, buf, count);
static void readChars(char *buf, size_t count);
static size_t sys_read(FDS fd, const char *buf, size_t count);
static void sys_sleep(int seconds);
static void sys_putPixel(uint32_t hexColor,uint64_t x,uint64_t y);
static void sys_setZoom(int new_zoom);

uint64_t sysCallDispatcher(uint64_t rax, ...);


static uint32_t color = 0xFFFFFF;
static int cursorX=0, cursorY=0;
static uint8_t zoom = 1;

static void writeFiles(FDS fd, const char *buf, size_t count) {
    // TODO
}

static drawRectangle(Point* topLeft, Point* downRigth, uint32_t c) {
    for (int i = topLeft->x; i < downRigth->x; i++) {
        for (int j = topLeft->y; j < downRigth->y; j++) {
            putPixel(c, i, j);
        }
    }
}

static size_t sys_setCursor(int x, int y) {
    cursorX = x;
    cursorY = y;
    return 1;
}

static void setFontColor(uint32_t hexColor) {
    color = hexColor;
}

static void sys_setZoom(int new_zoom) {
    zoom = new_zoom;
}

static void sys_write(FDS fd, const char *buf, size_t count) {
    if(fd == STDOUT || fd == STDERR) {
        int i = 0;
        char increase[] = {0, 1, 3};
        for( ; i < count; i++) {

            drawchar(buf[i], ((cursorX+i)*CHAR_WIDTH)%(DIM_X), (cursorY + ((cursorX+i)*CHAR_WIDTH*zoom)/DIM_X * zoom ) * CHAR_HEIGHT, (fd==STDOUT)?color:0xFF0000, 0x000000, zoom);
        }
        cursorX += i%(DIM_X/(CHAR_WIDTH*zoom));
    }
    return;
}

static void readFiles(fd, buf, count){
    // TODO
}

static void readChars(char *buf, size_t count) {
    for (size_t i = 0; i < count; i++) {
        char c=nextKey();
        if (c == -1) {
            i--;
        } else {
            buf[i] = c;
        }
    }
}

static size_t sys_read(FDS fd, const char *buf, size_t count) {
    if (fd == STDIN) {
        readChars(buf, count);
    } else {
        readFiles(fd, buf, count);
    }
    return count;
}

static void sys_sleep(int seconds){
    sleep(seconds);
}

static void sys_putPixel(uint32_t hexColor, uint64_t x,uint64_t y) {
    putPixel(hexColor, x, y);
}

uint64_t sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);
    uint64_t ret;
    if (rax == 0) {
        FDS fd = va_arg(args, FDS);
        const char* buf = va_arg(args, const char*);
        size_t count = va_arg(args, size_t);
        ret = sys_read(fd, buf, count);
    } else if (rax == 1) {
        FDS fd = va_arg(args, FDS);
        const char * buf = va_arg(args, const char*);
        size_t count = va_arg(args, size_t);
        sys_write(fd, buf, count);
        ret = 0;
    } else if (rax == 4) {
        ret=getTime();
    } else if (rax == 5) {
        int x = (int)va_arg(args, uint64_t);
        int y = (int)va_arg(args, uint64_t);
        sys_setCursor(x, y);
    } else if (rax == 6) {
        uint32_t hexColor = va_arg(args, uint32_t);
        uint64_t x = va_arg(args, uint64_t);
        uint64_t y = va_arg(args, uint64_t);
        sys_putPixel(hexColor, x, y);
    } else if (rax == 7) {
        uint32_t hexColor = va_arg(args, uint32_t);
        setFontColor(hexColor);
    } else if (rax == 8) {
        uint64_t new_zoom = va_arg(args, uint64_t);
        sys_setZoom(new_zoom);
    }else if (rax == 9) {
        Point* p1 = va_arg(args, Point*);
        Point* p2 = va_arg(args, Point*);
        uint32_t c = va_arg(args, uint32_t);
        drawRectangle(p1,p2,c);
        ret = 0;
    } else if (rax == 35) {
        int seconds = va_arg(args, int);
        sys_sleep(seconds);    
    }
    va_end(args);
    return ret;
}