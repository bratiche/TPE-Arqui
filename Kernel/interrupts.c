#include <types.h>
#include <keyboard.h>
#include <video.h>
#include <stdint.h>
#include <syscalls.h>
#include <interrupts.h>
#include <timer.h>

static SYSCALL syscalls[SYSCALLS_SIZE];		// array de punteros a funcion para las syscalls
static ISR routines[ISR_SIZE];				// arrays de punteros a funcion para las interrupciones

void irqDispatcher(uint32_t irq) {
	routines[irq]();
}

/*	
syscallHandler : called when instruction int 80 is executed:
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

void init_interrupts() {
	routines[PIT_IRQ] = timer_handler;
	routines[KB_IRQ] = keyboard_handler;

	syscalls[SYS_EXIT] = sys_exit;
	syscalls[SYS_READ] = sys_read;
	syscalls[SYS_WRITE] = sys_write;
}

