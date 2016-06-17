#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

static void printf_test(void);

int main() {

	//char * msg = "Hello,World!\n";

	// write(STDOUT, msg, strlen(msg));

	// write(STDERR, msg, strlen(msg));

	//exit(0); //funciona

	//printf_test();

	init_shell();

	while (1) {
		update_shell();
	}

	return 0;
}

void printf_test() {
	int i;
	printf("printf test:\n");
	i = printf("%s\n", "string");
	printf("characters written: %d\n", i);
	i = printf("%x\n", 10);
	printf("characters written: %d\n", i);
	i = printf("%d\n", -10);
	printf("characters written: %d\n", i);
	i = printf("%o\n", 10);
	printf("characters written: %d\n", i);
	i = printf("%b\n", 10);
	printf("characters written: %d\n", i);
	i = printf("%c %c\n", 'A', 'A' + UCHAR_MAX + 1);
	printf("characters written: %d\n", i);
}
