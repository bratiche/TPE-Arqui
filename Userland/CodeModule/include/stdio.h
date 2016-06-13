#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define EOF -1

char getchar(void);
char fgetc(int fd);
void putchar(int c);
void fputc(int fd, int c);

//gets
//fgets
int fputs(int fd, char * str);
int puts(char * str);
int printf(const char * fmt, ...);
int fprintf(int fd, const char * fmt, ...);
int vfprintf(int fd, const char * fmt, va_list ap);


#endif