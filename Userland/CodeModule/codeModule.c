#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell.h>

static void printf_test(void);

int main() {

	init_shell();

	// char buffer[50] = {0};

	// char c;
	// int i = 0;
	// // while ((c=fgetc(STDDATA)) != EOF) {
	// // 	buffer[i++] = c;
	// // 	putchar(c);
	// // }

	char * ptr = malloc(4);
	printf("%d\n", ptr);

	ptr = malloc(10);
	printf("%d\n", ptr);

	ptr = malloc(0);
	printf("%d\n", ptr);

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
