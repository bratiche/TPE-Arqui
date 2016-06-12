#include <string.h>

int strlen(const char * str) {
	int i = 0;
	while (*str++) { i++; }
	return i;
}