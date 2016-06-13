#include <stdio.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>
#include <commands.h>

#define BUFFER_SIZE 80
#define COMMANDS_SIZE 2

static void add_command(command_id id, const char * name, const char * desc, command_fn fn, int argc);

static void excute_command(void);
static void init_commands(void);
static void clean_buffer(void);
static void prompt(void);
static int is_empty(void);

char buffer[BUFFER_SIZE] = { 0 };
int current_pos = 0;

char * user_name = "user";
char * os_name = "undef";

command_t commands[COMMANDS_SIZE];

void init_shell() {
	//get_user_name();
	clean_buffer();
	init_commands();
	putchar('\n');
	prompt();
}

void update_shell() {
	char key = getchar();

	if (key <= 0) return;

	switch (key) {
		case '\n':
			if (current_pos != 0) {
				putchar('\n');
			}
			excute_command();
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

void init_commands(void) {
	add_command(ECHO, "echo", "echo [arg]", echo, 1);
	add_command(HELP, "help", "help [arg]?", help, 1);
}

void add_command(command_id id, const char * name, const char * desc, command_fn fn, int argc) {
	commands[id].id = id;
	commands[id].name = name;
	commands[id].desc = desc;
	commands[id].fn = fn;
	commands[id].argc = argc;
}

void excute_command() {
	if (is_empty()) {
		putchar('\n');
		return;
	}

	int i = 0;
	for (i = 0; i < COMMANDS_SIZE; i++) {
		// //TODO checkear que el name del command sea substring del buffer
		command_t command =  commands[i];
		// if (substr(command.name, buffer)) {
		// 	// TODO checkear codigo de retorno del comando
		// 	command.fn(buffer spliteado en espacios)
		// }
		if (strcmp(command.name, buffer) == 0) {
			return command.fn(0, 0);
		}
	}

	printf("Invalid command!\n");
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

int is_empty() {
	return buffer[0] == 0;
}