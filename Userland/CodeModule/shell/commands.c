#include <commands.h>
#include <stdio.h>


int echo(int argc, char ** argv) {
	if (argc != 1) {
		return -1;
	}
	return printf(argv[0]);
}

int help(int argc, char ** argv) {
	printf("help is comming!\n");
}