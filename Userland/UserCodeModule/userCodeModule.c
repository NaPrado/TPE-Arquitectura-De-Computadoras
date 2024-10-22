/* userCodeModule.c */

#include "./include/shell.h"
#include "./include/libc.h"

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {

    setCursor(10, 10);

    printf("Hello world!");
	
    shell();

}
