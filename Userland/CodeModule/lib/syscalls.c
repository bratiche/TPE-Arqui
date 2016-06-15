#include <syscalls.h>

extern int _syscall(int id, int arg1, int arg2, int arg3);

// static int syscall(int id, int arg1, int arg2, int arg3) {
// 	return _syscall(id, arg1, arg2, arg3);
// }

int exit(int code) {
	return _syscall(1, code, 0, 0);
}

int read(unsigned int fd, const void * buffer, unsigned int size) {
	return _syscall(3, fd, (unsigned long)buffer, size);
}

int write(unsigned int fd, const void * buffer, unsigned int size) {
	return _syscall(4, fd, (unsigned long)buffer, size);
}

int video(unsigned int width, unsigned int height, unsigned int bpp) {
	return _syscall(5, width, height, bpp);
}
