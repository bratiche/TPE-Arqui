#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <fractals.h>

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
		fputs(STDERR, "Too many arguments!\n");
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

static int video_mode_enabled = 0;

int fractal(int argc, char ** argv) {
	if (argc > 1) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}

	if (argc == 0) {
		printf("Available fractals: \n");
		printf("\t1- Mandelbrot\n");
		printf("\t2- Julia Set\n");
		return 0;
	}

	char * option = argv[0];

	if (strcmp(option, "1") == 0) {

		while (1) {
			char name[5] = {0}; 
			int iter, r, g, b, r2, g2, b2;
			int args = fscanf(STDDATA, "%s %d %d %d %d %d %d %d", name, &iter, &r, &g, &b, &r2, &g2, &b2);

			if (args != 8) {
				fprintf(STDERR, "Wrong format!\n");
				return -1;
			}
			if (!video_mode_enabled) {
				video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
				video_mode_enabled = 1;
			}

			mandelbrot(iter, r, g, b, r2, g2, b2);
		}
	}
	else if (strcmp(option, "2") == 0) {
		if (!video_mode_enabled) {
			video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
			video_mode_enabled = 1;
		}
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