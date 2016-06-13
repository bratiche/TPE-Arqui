#include <stdint.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

int main() {

	char * msg = "Hello,World!";

	write(STDOUT, msg, strlen(msg));

	write(STDERR, msg, strlen(msg));

	//exit(0); //funciona

	init_shell();

	while (1) {
		update_shell();
	}

	return 0;
}
