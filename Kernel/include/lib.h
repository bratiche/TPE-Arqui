#ifndef LIB_H
#define LIB_H

#include <stdint.h>

typedef enum {false = 0, true = 1} bool;

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * memsetw(void * destination, unsigned short val, int count);

char *cpuVendor(char *result);

#endif