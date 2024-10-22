#include <sysCallDispatcher.h>
#include <stdarg.h>
#include <videoDriver.h>
#include <time.h>
#include <videoDriver.h>
#include <keyboardDriver.h>


static void writeFiles(FDS fd, const char *buf, size_t count){
    //TODO
}
static int cursorX=0, cursorY=0;
size_t sys_setCursor(int x, int y){
    cursorX = x;
    cursorY = y;
    return 1;
}

size_t sys_write(FDS fd, const char *buf, size_t count){
    switch(fd){
        int i = 0;
        case STDOUT:
            for(; i < count; i++){
                drawchar(buf[i], cursorX+i*CHAR_WIDTH, cursorY*CHAR_HEIGHT, 0xFFFFFF, 0x000000);
            }
            cursorX += i*CHAR_WIDTH;
            break;
        case STDERR:
            for(; i < count; i++){
            drawchar(buf[i], cursorX+i*CHAR_WIDTH, cursorY*CHAR_HEIGHT, 0xFF0000, 0x000000);
            }
            cursorX += i*CHAR_WIDTH;
            break;
        default:
            //writeFiles(fd,buf, count);
            break;
    }
    return 1;
}

void readFiles(fd, buf, count){
    //TODO
}
void readChars(const char *buf, size_t count){
    //TODO
}

size_t sys_read(FDS fd, const char *buf, size_t count){
    switch(fd){
        case STDIN:
            readChars(buf, count);
            break;
        default:
            readFiles(fd, buf, count);
            break;
    }
    return count;
}

void sys_sleep(int seconds){
    sleep(seconds);
}

char sys_keyboard(char descriptor){
    switch (descriptor)
    {
    case 0:
        return getKey();
    case 1:
        return hasNextKey();
    case 2:
        return nextKey();
    case 3:
        return getCapslock(); 
    case 4:
        return getShiftPressed();
    case 5:
        return getCtrlPressed();
    case 6:
        return getAltPressed();
    default:
        return -1;
    }
}
void sys_putPixel(uint32_t hexColor,uint64_t x,uint64_t y){
    putPixel(hexColor, x, y);
}

char sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);
    switch(rax){
        case 0:;
            FDS fdr = va_arg(args, FDS);
            const char* bufr = va_arg(args, const char*);
            size_t countr = va_arg(args, size_t);
            return sys_read(fdr, bufr, countr);
        case 1:;
            FDS fdw = va_arg(args, FDS);
            const char* bufw = va_arg(args, const char*);
            size_t countw = va_arg(args, size_t);
            return sys_write(fdw, bufw, countw);
        case 4:;
            char descriptor = va_arg(args, char);
            return sys_keyboard(descriptor);
        case 5:;
            int xdot = va_arg(args, int);
            int ydot = va_arg(args, int);
            return sys_setCursor(xdot, ydot);
        case 6:;
            uint32_t hexColor = va_arg(args, uint32_t);
            uint64_t xPos = va_arg(args, uint64_t);
            uint64_t yPos = va_arg(args, uint64_t);
            sys_putPixel(hexColor, xPos, yPos);
            return 1;
        case 35:;
            int seconds = va_arg(args, int);
            sys_sleep(seconds);
            return seconds;
    }
    va_end(args);
    return;
}