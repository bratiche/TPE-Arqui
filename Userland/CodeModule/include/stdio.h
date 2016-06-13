#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define EOF -1
#define UCHAR_MAX 0xFF

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