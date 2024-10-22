/* userCodeModule.c */

#include "./include/shell.h"
#include "./include/libc.h"


int main() {

    setCursor(10, 10);

    printf("Hello world!");
	
    shell();

    return 0;
}
