/* userCodeModule.c */

#include <libc.h>

int main() {

    print("Hello");
    sleep(15);
    print("World");
    char* buf="------";
    scan(buf,7);
    putChar(" ");
    print(buf);
    return 0;
}
