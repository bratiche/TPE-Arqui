#include <syscalls.h>
#include <video.h>
#include <keyboard.h>
#include <bga.h>

extern void haltcpu(void); // Termina la ejecucion de la cpu.

static int sys_write(uint64_t fd, uint64_t buffer, uint64_t len);
static int sys_read(uint64_t fd, uint64_t buffer, uint64_t len);
static int sys_exit(uint64_t code, uint64_t arg2, uint64_t arg3);

static int sys_video(uint64_t width, uint64_t height, uint64_t bpp);
static int sys_draw(uint64_t x, uint64_t y, uint64_t color);

static SYSCALL syscalls[SYSCALLS_SIZE];		// array de punteros a funcion para las syscalls

/*	
syscallDispatcher : called when instruction int 80 is executed:
-------------- System Calls Table: ----------------

name	 	 rdi(number)	rsi		rdx 	rcx

sys_exit		1			code	-		-
sys_read		3			fd		buffer	size
sys_write		4			fd		buffer	size
---------------------------------------------------
 */
void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	syscalls[rdi](rsi, rdx, rcx);
}

/* Initializes software interrupts (syscalls) */
void init_syscalls() {
	syscalls[SYS_EXIT] = sys_exit;
	syscalls[SYS_READ] = sys_read;
	syscalls[SYS_WRITE] = sys_write;
	syscalls[SYS_VIDEO] = sys_video;
	syscalls[SYS_DRAW] = sys_draw;
}

/* Retorna la cantidad de caracteres escritos */
int sys_write(uint64_t fd, uint64_t buf, uint64_t size) {
	char * buffer = (char *)buf;
	int len = size;
	char attr;

	switch(fd) {
		case STDOUT:
			attr = LIGHT_GREY;
			break;
		case STDERR:
			attr = RED;
			break;
		default:
			return 0;
	}

	while(len--  && *buffer != 0) {
		putchar(*buffer, attr);
		buffer++;
	}

	return size - len - 1;
}

static int read_stdin(char * buffer, int len) {
	int i = 0;
	char c;

	while (i < len && !is_empty()) {
		c = get_key();
		buffer[i++] = c;
	}

	return i;
}

/* Retorna la cantidad de caracteres leidos */
int sys_read(uint64_t fd, uint64_t buf, uint64_t size) {
	char * buffer = (char *)buf;
	int len = size;

	switch (fd) {
		case STDIN:
			return read_stdin(buffer, len);
		default:  
			return 0;
	}
}

int sys_exit(uint64_t code, uint64_t arg2, uint64_t arg3) {
	puts("Exit code: ", LIGHT_GREY);
	putnumber(code, LIGHT_GREY);
	haltcpu();
	return code;
}

/* Starts VESA video mode with the given parameters */
int sys_video(uint64_t width, uint64_t height, uint64_t bpp) {
	puts("Starting video mode...", LIGHT_GREY);

	int rectWidth = 500;
	int rectHeight = 250;
	
	BgaSetVideoMode(width, height, 24, 1, 1);	// solo se acepta 24 bpp
	BgaDrawRect(0, 0, 0xff, width / 2 - rectWidth / 2, height / 2 - rectHeight / 2, rectWidth, rectHeight);
	BgaFillScreen(0x0, 0x0, 0x0);

	return 0;
}

/* Draws the pixel in the [x,y] position with the specified color */
int sys_draw(uint64_t x, uint64_t y, uint64_t color) {
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = color & 0xFF;

	return BgaDrawPixel(x, y, r, g, b);
}
