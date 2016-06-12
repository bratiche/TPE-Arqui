#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDOUT 1
#define STDERR 2

void write(unsigned int fd, const void * buffer, unsigned int size);
void read(unsigned int fd, const void * buffer, unsigned int size);
void exit(int code);

#endif