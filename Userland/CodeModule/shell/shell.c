#include <stdio.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>

#define BUFFER_SIZE 80

static void clean_buffer(void);
static void print_prompt(void);

char buffer[BUFFER_SIZE] = { 0 };
int current_pos = 0;

char * user_name = "user";
char * os_name = "undef";

void init_shell() {
	//get_user_name();
	clean_buffer();

	//reemplazar por printf
	putchar('\n');
	print_prompt();
}

void update_shell() {
	char key = getchar();

	if (key <= 0) return;

	switch (key) {
		case '\n':
			putchar('\n');
			puts(buffer);
			if (current_pos != 0) {
				putchar('\n');
			}
			//excute_command();
			print_prompt();
			clean_buffer();
			break;
		case '\b':
			if (current_pos > 0) {
				putchar('\b');	
				buffer[--current_pos] = 0;
			}	
			break;
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

void print_prompt() {
	puts(user_name);
	puts("@");
	puts(os_name);
	puts("> ");
}