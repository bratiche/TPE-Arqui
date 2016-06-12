#include <stdio.h>
#include <syscalls.h>

void putchar(int c) {
	write(STDOUT, &c, 1);
}