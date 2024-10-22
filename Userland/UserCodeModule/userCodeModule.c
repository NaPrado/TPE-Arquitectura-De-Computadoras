/* userCodeModule.c */

#include "./include/shell.h"
#include "./include/libc.h"


int main() {

    setCursor(0X100, 0X100);

    printf("Hello world!");
	
    shell();

    return 0;
}
