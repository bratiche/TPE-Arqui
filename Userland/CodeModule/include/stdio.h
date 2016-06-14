#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define EOF -1
#define UCHAR_MAX 0xFF
#define NULL 0

#define STDIN	0
#define STDOUT	1
#define STDERR	2

int getchar(void);
int fgetc(int fd);
void putchar(char c);
void fputc(int fd, char c);

//gets
//fgets
int fputs(int fd, char * str);
int puts(char * str);
int printf(const char * fmt, ...);
int fprintf(int fd, const char * fmt, ...);
int vfprintf(int fd, const char * fmt, va_list ap);


#endif