#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN	0
#define STDOUT	1
#define STDERR	2

int exit(int code);

/* Retorna la cantidad de caracteres escritos */
int write(unsigned int fd, const void * buffer, unsigned int size);
/* Retorna la cantidad de caracteres leidos */
int read(unsigned int fd, const void * buffer, unsigned int size);
/* Entra en modo video con los parametros dados */
int video(unsigned int width, unsigned int height, unsigned int bpp);
/* Pinta el pixel x,y */
int draw(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b);

#endif