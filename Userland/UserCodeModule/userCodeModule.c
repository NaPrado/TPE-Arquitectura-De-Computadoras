/* userCodeModule.c */

#include <libc.h>

int main() {

    print("Hello");
    //sleep(5);
    print("World");
    char* buf="------";
    scan(buf,7);
    putChar(" ");
    print(buf);
    return 0;
}
