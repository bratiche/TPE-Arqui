#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

static void printf_test(void);

int main() {

	char * msg = "Hello,World!\n";

	write(STDOUT, msg, strlen(msg));

	write(STDERR, msg, strlen(msg));

	//exit(0); //funciona

	printf_test();

	init_shell();

	while (1) {
		update_shell();
	}

	return 0;
}


void printf_test() {
	printf("%s\n", "string");
	printf("hexa: %x\n", 10);
	printf("decimal: %d\n", 10);
	printf("octal: %o\n", 10);
	printf("binario: %b\n", 10);
}
