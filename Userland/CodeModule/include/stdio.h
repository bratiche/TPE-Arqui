#ifndef STDIO_H
#define STDIO_H

#define EOF -1

char getchar();
char fgetc(int fd);
void putchar(int c);
void puts(char * str);
int printf(const char * ftm, ...);

#endif