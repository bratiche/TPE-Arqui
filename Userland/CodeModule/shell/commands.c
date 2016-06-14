#include <commands.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>

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