#include <stdio.h>
#include <syscalls.h>

void putchar(int c) {
	write(STDOUT, &c, 1);
}

char fgetc(int fd) {
	char c;
	read(fd, &c, 1);
	return c;
}

char getchar() {
	return fgetc(STDIN);
}

void puts(char * str) {
	while (*str) {
		putchar(*str);
		str++;
	}
}

//TODO
int printf(const char * ftm, ...) {
	return 0;
}
