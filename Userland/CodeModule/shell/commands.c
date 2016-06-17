#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <fractals.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_BPP 24

static char * user = "user"; // TODO cuando implemente mayusculas en el driver del teclado esto tiene que ser $USER

static char username[MAX_USERNAME_SIZE];

int echo(int argc, char ** argv) {
	int i = 0;
	char * arg;

	if (argc == 0) {
		fputs(STDERR, "No arguments!\n");
		return -1;
	}

	for (; i < argc; i++) {
		arg = argv[i];
		if (strcmp(user, arg) == 0) {
			get_username(username);
			printf("%s ", username);
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

	return printf("help is comming!\n");
}

int start_video(int argc, char ** argv) {
	return video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);
}

int fractal(int argc, char ** argv) {
	if (argc != 1) {
		fputs(STDERR, "Invalid arguments!\n");
		return -1;
	}

	char * option = argv[0];

	if (strcmp(option, "1") == 0) {
		printf("fractal 1\n");
		//mandelbrot();
	}
	else if (strcmp(option, "2") == 0) {
		start_video(0, 0);
		mandelbrot2();
	}
	else if (strcmp(option, "3") == 0) {
		start_video(0, 0);
		mandelbrot3();
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