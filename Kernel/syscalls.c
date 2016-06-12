#include <syscalls.h>
#include <video.h>
#include <keyboard.h>

extern void haltcpu(void); // Termina la ejecucion de la cpu.

static SYSCALL syscalls[SYSCALLS_SIZE];		// array de punteros a funcion para las syscalls

/*	
syscallDispatcher : called when instruction int 80 is executed:
-------------- System Calls Table: ----------------

name	 	 rax(number)	rbx		rcx 	rdx

sys_exit		1			code	-		-
sys_read		3			fd		buffer	size
sys_write		4			fd		buffer	size
---------------------------------------------------
 */
void syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	syscalls[rax](rbx, rdx, rcx);
}

/* Initializes software interrupts (syscalls) */
void init_syscalls() {
	syscalls[SYS_EXIT] = sys_exit;
	syscalls[SYS_READ] = sys_read;
	syscalls[SYS_WRITE] = sys_write;
}

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

static void read_stdin(char * buffer, int len) {
	int i, c;
	for (i = 0; i < len && (c = get_key()) != -1; i++) {
		buffer[i] = (char)c;
	}
}

void sys_read(uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	int fd = rbx;
	char * buffer = (char *)rcx;
	int len = rdx;

	switch (fd) {
		case STDIN:
			read_stdin(buffer, len);
			return;
		default:  
			return;
	}

}

void sys_exit(uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	puts("Exit code: ", LIGHT_GREY);
	putnumber(rbx, LIGHT_GREY);
	haltcpu();
}