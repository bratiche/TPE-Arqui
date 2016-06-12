#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>

int main() {

	char * msg = "Hello,World!";

	write(STDOUT, msg, strlen(msg));

	write(STDERR, msg, strlen(msg));

	putchar(getchar());

	//exit(0); //funciona

	while (1);

	return 0;
}

