#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <fractals.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_BPP 24

static char * user = "USER";

static char username[MAX_USERNAME_SIZE];

int echo(int argc, char ** argv) {
	int i;
	char * arg;

	for (i = 0; i < argc; i++) {
		arg = argv[i];
		if (arg[0] == '$') {
			if (strcmp(user, arg + 1) == 0) {
				get_username(username);
				printf("%s ", username);
			}
		}
		else {
			printf("%s ", arg);
		}
	}

	putchar('\n');

	return 0;
}

int help(int argc, char ** argv) {
	if (argc > 1) {
		fputs(STDERR, "Too much arguments!\n");
		return -1;
	}

	if (argc == 1) {
		char * option = argv[0];
		command_t command;
		int i, found = 0;
			
		for (i = 0; i < COMMANDS_SIZE; i++) {
		 	command = get_command(i);
			if (strcmp(command.name, option) == 0) {
				printf("%s: %s\n", command.name, command.desc);
				found = 1;
				break;
			}
		}

		if (!found) {
			fprintf(STDERR, "Invalid argument: '%s'\n", option);
			return -1;	
		}

		switch (command.id) {
			case ECHO:	
				printf("\tWrite arguments to the standard output, separated by a single space.\n");
				break;
			case HELP:
				printf("\tDisplays information about commands.\n");
				printf("\tIf no command is specified, prints a list of available commands.\n");
				break;
			case FRACTAL:
				printf("\tStarts video mode and draws the specified fractal.\n");
				break;
			case CLEAR:
				printf("\tClears the console.\n");
				break;
			case EXIT:
				printf("\tExits the shell.\n");
				break;
		}

		return 0;
	}

	int i;

	for (i = 0; i < COMMANDS_SIZE; i++) {
		command_t command = get_command(i);
		printf("%s\n", command.desc);
	}

	return 0;
}

int fractal(int argc, char ** argv) {
	if (argc > 1) {
		fputs(STDERR, "Invalid arguments!\n");
		return -1;
	}

	char * option = argv[0];

	if (strcmp(option, "1") == 0) {
		video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
		//mandelbrot(30, 0xff, 0xff, 0xff, 0, 0xff, 0);
		mandelbrot(30, 0, 0, 0, 0xff, 0, 0);
	}
	else if (strcmp(option, "2") == 0) {
		video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
		mandelbrot2();
	}
	else if (strcmp(option, "3") == 0) {
		video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);
		juliaSet();
	}
	else {
		fprintf(STDERR, "Invalid option '%s'\n", option);
		return -1;
	}

	return 0;
}

#define LINES 25

int clear(int argc, char ** argv) {
	int i;
	for (i = 0; i < LINES  - 1; i++) {
		putchar('\n');
	}
	
	putchar('\b');

	return 0;
}

int _exit(int argc, char ** argv) {
	return exit(0);
}