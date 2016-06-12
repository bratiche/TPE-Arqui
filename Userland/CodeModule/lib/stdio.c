#include <stdio.h>
#include <syscalls.h>

void putchar(int c) {
	write(STDOUT, &c, 1);
}

int getchar() {
	int c;
	read(STDIN, &c, 1);
	return c;
}