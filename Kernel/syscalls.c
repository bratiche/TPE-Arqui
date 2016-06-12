#include <syscalls.h>
#include <interrupts.h>
#include <video.h>

void sys_write(uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	int fd = rbx;
	char * buffer = (char *)rcx;
	int len = rdx;

	char attr;
	switch(fd) {
		case STDOUT:
			attr = LIGHT_GREY;
			break;
		case STDERR:
			attr = RED;
			break;
		default:
			return;
	}

	while(len--) {
		putchar(*((char *)buffer), attr);
		buffer++;
	}
}

void sys_read(uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	int fd = rbx;
	char * buffer = (char *)rcx;
	int len = rdx;

	int i = 0;
	for (i = 0; i < len; i++) {
		buffer[i] = 'A';
	}

}

void sys_exit(uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	puts("Exit code: ", LIGHT_GREY);
	putnumber(rbx, LIGHT_GREY);
	haltcpu();
}