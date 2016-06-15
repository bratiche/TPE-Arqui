#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_BPP 24

static char * user = "user"; // TODO cuando implemente mayusculas en el driver del teclado esto tiene que ser $USER

static char username[MAX_USERNAME_SIZE];

int echo(int argc, char ** argv) {
	if (argc != 1) {
		fputs(STDERR, "Invalid arguments!\n");
		return -1;
	}

	if (strcmp(user, argv[0]) == 0) {
		get_username(username);
		return printf("%s\n", username);
	}
	return printf("%s\n", argv[0]);
}

int help(int argc, char ** argv) {
	if (argc > 1) {
		return -1;
	}
	return printf("help is comming!\n");
}

int start_video(int argc, char ** argv) {
	return video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);
}
