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
        case STDOUT:
            for(int i = 0; i < count; i++){
                drawchar(buf[i], cursorX+i*CHAR_WIDTH, cursorY*CHAR_HEIGHT, 0xFFFFFF, 0x000000);
            }
            break;
        case STDERR:
            //printChars(buf, count, 0xFF0000, 0x000000);
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

void sys_keyboard(char descriptor){
    switch (descriptor)
    {
    case 0:
        getKey();
        break;
    case 1:
        hasNextKey();
        break;
    case 2:
        nextKey();
        break;
    case 3:
        getCapslock();
        break;  
    case 4:
        getShiftPressed();
        break;
    case 5:
        getCtrlPressed();
        break;
    case 6:
        getAltPressed();
        break;
    default:
        break;
    }
}
sys_video(){
    //TODO
}

void sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);
    switch(rax){
        case 0:;
            FDS fdr = va_arg(args, FDS);
            const char* bufr = va_arg(args, const char*);
            size_t countr = va_arg(args, size_t);
            sys_read(fdr, bufr, countr);
            break;
        case 1:;
            FDS fdw = va_arg(args, FDS);
            const char* bufw = va_arg(args, const char*);
            size_t countw = va_arg(args, size_t);
            sys_write(fdr, bufw, countw);
            break;
        case 4:;
            char descriptor = va_arg(args, char);
            sys_keyboard(descriptor);
            break;
        case 5:;
            int x = va_arg(args, int);
            int y = va_arg(args, int);
            sys_setCursor(x, y);
            break;
        case 35:;
            int seconds = va_arg(args, int);
            sys_sleep(seconds);
            break;
    }
    va_end(args);
    return;
}