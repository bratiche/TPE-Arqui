#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYSCALLS_SIZE 10

#define SYS_EXIT 	1
#define SYS_READ 	3
#define SYS_WRITE 	4

typedef void (*SYSCALL)(uint64_t rbx, uint64_t rcx, uint64_t rdx);	// puntero a funcion de manejo de una syscall

void init_syscalls(void);
void sys_write(uint64_t fd, uint64_t buffer, uint64_t len);
void sys_read(uint64_t fd, uint64_t buffer, uint64_t len);
void sys_exit(uint64_t fd, uint64_t buffer, uint64_t len);

#endif