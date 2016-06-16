#include <syscalls.h>

extern int _syscall(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3);

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

int draw(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
	unsigned int color = 0;
	color += r;
	color = color << 8;
	color += g;
	color = color << 8;
	color += b;
	return _syscall(6, x, y, color);
}