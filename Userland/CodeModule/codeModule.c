#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

static void printf_test(void);

int main() {

	init_shell();

	char buffer[50] = {0};

	read(STDDATA, buffer, 50);
	puts(buffer);

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
