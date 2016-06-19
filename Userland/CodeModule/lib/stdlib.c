#include <stdlib.h>
#include <syscalls.h>

void * malloc(uint64_t size) {
	void * ret = sbrk(0);	// pido la direccion actual
	sbrk(size);				// "incremento" el segmento de datos
	return ret;
}

void * calloc(uint64_t size) {
	int i;
	char * ret = malloc(size);

	for (i = 0; i < size; i++) {
		ret[i] = 0;
	}

	return (void *)ret;
}

void free(void * ptr) {
	return;
}