#include <string.h>

int strlen(const char * str) {
	int i = 0;
	while (*str++) { i++; }
	return i;
}

int strcmp(const char * s1, const char * s2) {

	if(*s1 == 0) {
		return *s2 != 0 ? -1 : 0;
	}
	if(*s2 == 0) {
		return 1;
	}

	int cmp = (*s1 > *s2)? 1 : ((*s1 < *s2)? -1 : 0);

	if(cmp != 0) {
		return cmp;
	}

	return strcmp(++s1, ++s2);
}

int strcpy(const char * from, char * to) {
	while(*from) {
		*to = *from;
		to++;
		from++;
	}
	*to = 0;
	return 0;
}