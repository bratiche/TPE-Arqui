#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <video.h>
#include <interrupts.h>
#include <syscalls.h>

#include <bga.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const codeModuleAddress = (void*)0x400000;
static void * const dataModuleAddress = (void*)0x500000;

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
		codeModuleAddress,
		dataModuleAddress
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


void update_video(void);
void update_buffer(void);
#define WIDTH 800
#define HEIGHT 600
#define BPP 24

void drawPixel(int x, int y, int r, int g, int b);

void drawRect(int r, int g, int b, int w, int h, int x, int y);


int main()
{	
	//ncPrint("[Kernel Main]");
	//ncNewline();
	//ncPrint("  Sample code module at 0x");
	//ncPrintHex((uint64_t)sampleCodeModuleAddress);
	//ncNewline();
	//ncPrint("  Calling the sample code module returned: ");
	//ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	//ncNewline();
	//ncNewline();

	clear();
	init_syscalls();	// set software interrupt handler functions
	init_interrupts();	// set hardware interrupt handler functions 

	puts("Bienvenido al kernelino\n", MAGENTA);

	BgaGetCapabilities();

	BgaSetVideoMode(WIDTH,HEIGHT,BPP,1,0);

	update_buffer();
	drawPixel(20, 20, 0xff, 0xff, 0xff);
	drawPixel(21, 20, 0xff, 0xff, 0xff);
	drawPixel(22, 20, 0xff, 0xff, 0xff);

	drawRect(0xff, 0xff, 0xff, 100, 20, 10, 10);

	while(1) {
		//update_video();
	}

	((EntryPoint)codeModuleAddress)();

	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)dataModuleAddress);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)dataModuleAddress);

	return 0;
}


unsigned char * bankAddress = (unsigned char *)0xA0000; 

//TODO hacer extern y global en el sysvar.asm
unsigned char ** addressAddress = (unsigned char **)(0x0000000000005C00 + 40);

// con linear frame buffer
void update_buffer() {
	int i = 0;
	unsigned char * linearBuffer = *addressAddress;

	for (i = 0; i < WIDTH * HEIGHT * 3; ) { // VBE_DISPI_TOTAL_VIDEO_MEMORY_BYTES se pasa de largo
		linearBuffer[i++] = 0;
		linearBuffer[i++] = 0;
		linearBuffer[i++] = 0;
	}
}

// con banks
void update_video() {
	int i, j;
	int size = 20;

	for (j = 0; j < size; j++) {
		BgaSetBank(j);
		for (i = 0; i < VBE_DISPI_BANK_SIZE_KB * 1024; i++) {
			bankAddress[i++] = 0;				//blue
			bankAddress[i++] = 0xff;			//green
			bankAddress[i] = 0;					//red
		}
	}
}

void drawPixel(int x, int y, int r, int g, int b) {
	unsigned char * linearBuffer = *addressAddress;
	int pos = x * WIDTH * 3 + y * 3;

	linearBuffer[pos++] = b;
	linearBuffer[pos++] = g;
	linearBuffer[pos] = r;
}

void drawRect(int r, int g, int b, int w, int h, int x, int y) {
	int i, j;

	for (i = x; i < w * 3; i++) {
		for (j = y; j < h * 3; j++) {
			drawPixel(i, j, r, g, b);
		}
	}

}