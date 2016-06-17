#include <stdio.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>
#include <commands.h>
#include <ctype.h>

#define BUFFER_SIZE 80

#define ignore_spaces(str) { while (isspace(*str)) str++; }
#define ignore_characters(str) { while (*str != 0 && !isspace(*str)) str++;}

static void add_command(command_id id, const char * name, const char * desc, command_fn fn);
static int prefix(const char * str1, const char * str2);
static int parse_args(char ** argv, char * str);

static void _get_username(void);
static void excute_command(void);
static void init_commands(void);
static void clean_buffer(void);
static void prompt(void);
static int is_empty(void);

static char buffer[BUFFER_SIZE] = { 0 };
static int current_pos = 0;

static char username[MAX_USERNAME_SIZE];
static char * os_name = "undef";

command_t commands[COMMANDS_SIZE];

void init_shell() {
	_get_username();
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
		case '\t':
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

void _get_username(void) {
	int i = 0;
	char c;

	do {
		clean_buffer();
		printf("\nPlease enter your username (up to %d characters): ", MAX_USERNAME_SIZE);
		while ((c = getchar()) != '\n') {
			if (c == '\b') {
				if (i > 0) {
					putchar('\b');
					buffer[--i] = 0;
				}
			}
			else if (c > 0 && !isspace(c)) {
				putchar(c);
				buffer[i++] = c;
			}
		}
		if (i > MAX_USERNAME_SIZE) {
			i = 0;
			fprintf(STDERR, "\nToo long...");
		}
		else if (i > 0) {
			strcpy(buffer, username);
		}
	} while (i <= 0 || i > MAX_USERNAME_SIZE);

}

void init_commands(void) {
	add_command(ECHO, "echo", "echo [arg]", echo);
	add_command(HELP, "help", "help [arg]?", help);
	add_command(VIDEO, "video", "video [width] [height] [bpp]", start_video);
	add_command(FRACTAL, "fractal", "fractal [number]", fractal);
	add_command(CLEAR, "clear", "clear", clear);
	add_command(EXIT, "exit", "exit", _exit);
}

void add_command(command_id id, const char * name, const char * desc, command_fn fn) {
	commands[id].id = id;
	commands[id].name = name;
	commands[id].desc = desc;
	commands[id].fn = fn;
}

void excute_command() {
	int i = 0;

	if (is_empty()) {
		putchar('\n');
		return;
	}

	for (i = 0; i < COMMANDS_SIZE; i++) {
		command_t command =  commands[i];
		int len = prefix(command.name, buffer);

		if (len > 0) {
			char * argv[MAX_ARGS];
			char * args_start = buffer + len;
			int argc;

			if (*args_start != '\0' && !isspace(*args_start)) {	//	habia otro caracter luego del comando, sin separacion
				break;
			}

			argc = parse_args(argv, args_start);

			command.fn(argc, argv);
			return;
		}
	}

	fprintf(STDERR, "Invalid command!\n");
}

// TODO move to string.c
/* Returns the lenght of str1 if str1 is prefix of str2, else returns 0 */
int prefix(const char * str1, const char * str2) {
	int i = 0;

	while (str1[i] != 0) {
		if (str2[i] == 0 || str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}

	return i;
}

int parse_args(char ** argv, char * str) {
	int i = 0;
	char * arg;
	
	ignore_spaces(str);
	while (*str != NULL) {
		arg = str;
		ignore_characters(str);
		*str++ = 0;
		argv[i++] = arg;
		ignore_spaces(str);
	}

	return i;
}

void clean_buffer() {
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	current_pos = 0;
}

void prompt() {
	printf("%s@%s> ", username, os_name);
}

int is_empty() {
	return buffer[0] == 0;
}

void get_username(char * buffer) {
	strcpy(username, buffer);
}