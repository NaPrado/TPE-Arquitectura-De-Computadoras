#include <sysCallDispatcher.h>
#include <stdarg.h>
#include <videoDriver.h>

struct cursor{
    int x;
    int y;
};
static struct cursor cursor = {0,0};

static void printChars(const char *buf,size_t count, int fgcolor, int bgcolor){
    for(size_t i = 0; i < count; i++){
        drawchar(buf[i], cursor.x, cursor.y, fgcolor, bgcolor);
        cursor.x=cursor.x+i*8;
    }
}

static void writeFiles(FDS fd, const char *buf, size_t count){
    //TODO
}


size_t sys_write(FDS fd, const char *buf, size_t count){
    switch(fd){
        case STDOUT:
            printChars(buf,count,0xFFFFFF,0x000000);
            break;
        case STDERR:
            printChars(buf, count, 0xFF0000, 0x000000);
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

void sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);
    switch(rax){
    case 1:;
        FDS fd = va_arg(args, FDS);
        const char* buf = va_arg(args, const char*);
        size_t count = va_arg(args, size_t);
        sys_write(fd, buf, count);
        break;
    }
    va_end(args);
    return;
}