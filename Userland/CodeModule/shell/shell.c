#include <stdio.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>

#define BUFFER_SIZE 80

static void clean_buffer(void);
static void prompt(void);

char buffer[BUFFER_SIZE] = { 0 };
int current_pos = 0;

char * user_name = "user";
char * os_name = "undef";

void init_shell() {
	//get_user_name();
	clean_buffer();

	putchar('\n');
	prompt();
}

void update_shell() {
	char key = getchar();

	if (key <= 0) return;

	switch (key) {
		case '\n':
			putchar('\n');
			printf(buffer);
			if (current_pos != 0) {
				putchar('\n');
			}
			//excute_command();
			prompt();
			clean_buffer();
			break;
		case '\b':
			if (current_pos > 0) {
				putchar('\b');	
				buffer[--current_pos] = 0;
			}	
			break;
		case ' ':		// no imprimo ni agrego al buffer los espacios iniciales
			if (current_pos == 0) {
				return;
			}
		default:
			putchar(key);
			if (current_pos == BUFFER_SIZE) {
				current_pos = 0;
			}
			buffer[current_pos++] = key;
			break;
	}
}

void clean_buffer() {
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	current_pos = 0;
}

void prompt() {
	printf("%s@%s> ", user_name, os_name);
}