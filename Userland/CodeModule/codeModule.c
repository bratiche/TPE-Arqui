#include <stdint.h>
#include <syscalls.h>
#include <string.h>

int main() {

	char * msg = "Hello,World!";

	write(STDOUT, msg, strlen(msg));

	write(STDERR, msg, strlen(msg));

	//exit(0); //funciona

	return 0;
}

