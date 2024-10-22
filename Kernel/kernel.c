#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <keyboardDriver.h>
#include <shell.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const userCodeModuleAddress = (void*)0x400000;
static void * const userDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		userCodeModuleAddress,
		userDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main() {	
	load_idt();

    ((EntryPoint)userCodeModuleAddress)();

    return 0;

	int i=0;
	char cursorY=0;
	char cursorx=0;
	// for (size_t j = '!'; j < 256; j++) {

    //     putchar(j);
    //     redrawScreen();
		
	// 	// drawchar(j, cursorx*8, cursorY, 0xffffffff, 0x00);
	// 	// cursorx++;
	// 	// if (j%115==0){
	// 	// 	cursorY+=16;
	// 	// 	cursorx=0;
	// 	// }
	// }

    

    
	

	while (1){	
		
		if (hasNextKey()){	
			char caps=(getShiftPressed()^getCapslock());
			char key=nextKey();
			char c=keyToAscii(key);
			char isLowercase=(c>='a' && c<='z');
			if (key==0x1C){// Enter
				i=0;
			}
			if (key==0x0E){// Backspace
					i--;
					drawchar(' ', 0x100 + i*8, 0x100, 0xffffffff, 0x00);
			} else {
                putchar(c - (caps&&isLowercase ? ('a'-'A'):0), 0, 0);
			    //drawchar(caps&&isLowercase?c-('a'-'A'):c, 0x100 + i*8, 0x100, 0xffffffff, 0x00);
			    i++;
			}
		}

        redrawScreen();
		
	}
	

	return 0;
}
