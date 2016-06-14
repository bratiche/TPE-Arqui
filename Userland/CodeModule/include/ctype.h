#ifndef CTYPES_H
#define CTYPES_H

int isalnum(int c);
int isalpha(int c);
int isdigit(int c);
int islower(int c);
int isprint(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int tolower(int c);
int toupper(int c);

//This function checks whether the passed character has graphical representation using locale.
int isgraph(int c);
//This function checks whether the passed character is a punctuation character.
int ispunct(int c);
//This function checks whether the passed character is control character.
int iscntrl(int c);

#endif