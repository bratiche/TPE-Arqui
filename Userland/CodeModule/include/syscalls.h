#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDIN	0
#define STDOUT	1
#define STDERR	2

/* Retorna la cantidad de caracteres escritos */
int write(unsigned int fd, const void * buffer, unsigned int size);
/* Retorna la cantidad de caracteres leidos */
int read(unsigned int fd, const void * buffer, unsigned int size);
int exit(int code);

#endif