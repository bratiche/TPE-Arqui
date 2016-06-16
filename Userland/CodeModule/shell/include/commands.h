#ifndef COMMAND_H_
#define COMMAND_H_

#define MAX_ARGS 10

typedef enum {
	ECHO,
	HELP,
	VIDEO,
	FRACTAL,
} command_id;

typedef int (*command_fn)(int, char **);

typedef struct {
	command_id id;
	const char * name;	// string to identify the command when parsing
	const char * desc;	// brief description on how to use the command
	int argc;		// cantidad maxima de argumentos que recibe
	command_fn fn;	// funcion de ejecucion del comando
} command_t;

int echo(int argc, char ** argv);
int help(int argc, char ** argv);
int start_video(int argc, char ** argv);
int fractal(int argc, char ** argv);

#endif
