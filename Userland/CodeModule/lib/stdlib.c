#include <stdlib.h>

static char * mem_start = 5000;	// start of data module, todo syscall to retrieve this address

void * malloc(uint64_t size) {
	char * ret = mem_start;
	mem_start += size;
	return (void *)ret;
}

void free(void * ptr) {
	return;
}