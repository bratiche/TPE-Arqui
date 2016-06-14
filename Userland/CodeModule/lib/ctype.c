#include <ctype.h>

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
	return c >= 'A' && c <= 'z';
}

int isdigit(int c) {
	return c >= '0' && c <= '9';
}

int islower(int c) {
	return c >= 'a' && c <= 'z';
}

int isprint(int c) {
	return c;
}

int isspace(int c) {
	return c == ' ' || c == '\t';
}

int isupper(int c) {
	return c >= 'A' && c <= 'Z';
}

int isxdigit(int c) {
	return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int tolower(int c) {
	if (islower(c)) {
		return c;
	}
	return c + ('a' - 'A');
}

int toupper(int c) {
	if (isupper(c)) {
		return c;
	}
	return c - ('a' - 'A');
}

//GG
int iscntrl(int c) {
	return 0;
}

int isgraph(int c) {
	return 0;
}

int ispunct(int c) {
	return 0;
}