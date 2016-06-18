#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define EOF -1
#define UCHAR_MAX 0xFF
#define NULL 0

#define STDIN	0
#define STDOUT	1
#define STDERR	2

void putchar(char c);
void fputc(int fd, char c);
int puts(char * str);
int fputs(int fd, char * str);
int fputsn(int fd, char * str, int n);
int printf(const char * fmt, ...);
int fprintf(int fd, const char * fmt, ...);
int vfprintf(int fd, const char * fmt, va_list ap);

int getchar(void);
int fgetc(int fd);
char * gets(char * str, int n);
char * fgets(int fd, char * str, int n);
int scanf(const char * fmt, ...);
int fscanf(int fd, const char * fmt, ...);
int sscanf(const char * str, const char * fmt, ...);
int vfscanf(int fd, const char * fmt, va_list ap);

#endif