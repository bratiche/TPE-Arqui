#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
			case TIME:
				printf("\tEnables/disables time in the top right corner of the shell.\n");
				break;
			case DATE:
				printf("\tPrints the date in the console.\n");
				break;
			case SET_TIME:
				printf("\tSets the time with the given parameters\n");
				break;
			case SET_DATE:
				printf("\tSets the date with the given parameters\n");
				break;
			case SLEEP:
				printf("\tSleeps for the given milliseconds.\n");
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

			video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
			mandelbrot(iter, r, g, b, r2, g2, b2);
		}
	}
	else if (strcmp(option, "2") == 0) {
		video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
		juliaSet();
	}
	else {
		fprintf(STDERR, "Invalid option '%s'\n", option);
		return -1;
	}

	return 0;
}

int _clear(int argc, char ** argv) {
	clear();
	return 0;
}

int _exit(int argc, char ** argv) {
	return exit(0);
}

int _time(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	time();
	return 0;
}

int _date(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	char * d = date();
	printf("%s\n", d);
	return 0;
}

//TODO use sscanf instead of atoi to validate the parameters
int _set_time(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(STDERR, "Invalid arguments!\n");
		return -1;
	}

	int hour, minutes, seconds;
	
	// sscanf(argv[0], "%d", &hour);
	// sscanf(argv[1], "%d", &minutes);
	// sscanf(argv[2], "%d", &seconds);
	hour = atoi(argv[0]);
	minutes = atoi(argv[1]);
	seconds = atoi(argv[2]);

	return set_time(hour, minutes, seconds);
}

int _set_date(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(STDERR, "Too few arguments!\n");
		return -1;
	}
	int day, month, year;

	// sscanf(argv[0], "%d", &day);
	// sscanf(argv[1], "%d", &month);
	// sscanf(argv[2], "%d", &year);
	day = atoi(argv[0]);
	month = atoi(argv[1]);
	year = atoi(argv[2]);

	return set_date(day, month, year);
}

int sleep(int argc, char ** argv) {
	if (argc != 1) {
		fprintf(STDERR, "Invalid arguments!\n");
		return -1;
	}

	unsigned long millis;

	// sscanf(argv[0], "%d", &millis);
	millis = atoi(argv[0]);
	printf("Sleeping for %d milliseconds...\n", millis);

	return wait(millis);
}