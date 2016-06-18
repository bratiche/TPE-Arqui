#include <stdio.h>
#include <string.h>
#include <syscalls.h>
#include <stdarg.h>

static int print_arg(int fd, va_list ap, char option);
static int print_hex(int fd, int hex);
static int print_dec(int fd, int dec);
static int print_oct(int fd, int oct);
static int print_bin(int fd, int bin);
static int num_to_base(unsigned int value, char * buffer, unsigned int base);

void putchar(char c) {
	fputc(STDOUT, c);
}

void fputc(int fd, char c) {
	write(fd, &c, 1);
}

int puts(char * str) {
	return fputs(STDOUT, str);
}

int fputs(int fd, char * str) {
	char c;
	int written = 0;
	while((c = *str++) != 0) {
		fputc(fd, c);
		written++;
	}
	return written;
}

int fputsn(int fd, char * str, int n) {
	return write(fd, str, n);
}

int printf(const char * fmt, ...) {
	va_list ap;
	int written = 0;

	va_start(ap, fmt);
	written += vfprintf(STDOUT, fmt, ap);
	va_end(ap);

	return written;
}

int fprintf(int fd, const char * fmt, ...) {
	va_list ap;
	int written = 0;

	va_start(ap, fmt);
	written += vfprintf(fd, fmt, ap);
	va_end(ap);

	return written;	
}

int vfprintf(int fd, const char * fmt, va_list ap) {
	int written = 0;
	char c;

	while ((c = *fmt++) != 0) {
		if (c == '%') {
			char option = *(fmt++);
			written += print_arg(fd, ap, option);
		}
		else {
			fputc(fd, c);
			written++;
		}
	}

	return written;
}

static int print_arg(int fd, va_list ap, char option) {
	int written = 0;

	switch (option) {
		case 's':
			written += fputs(fd, va_arg(ap, char *));
			break;
		case 'x':
			written += print_hex(fd, va_arg(ap, int));
			break;
		case 'd':
			written += print_dec(fd, va_arg(ap, int));
			break;
		case 'o':
			written = print_oct(fd, va_arg(ap, int));
			break;
		case 'b':
			written = print_bin(fd, va_arg(ap, int));
			break;
		case 'c':
			fputc(fd, va_arg(ap, int) & UCHAR_MAX);
			written++;
			break;
		default:
			return written;		// error de formato
	}

	return written;
}

static int print_hex(int fd, int num) {
	int digits, written;
	char buffer[20];
	digits =  num_to_base(num, buffer, 16);
	written = fputsn(fd, buffer, digits);
	// fputc(fd, 'h');
	// written++;
	return written;
}

static int print_dec(int fd, int num) {
	int digits;
	int negative = 0;
	char buffer[20];
	if (num < 0) {
		fputc(fd, '-');
		negative = 1;
		num *= -1;
	}
	digits = num_to_base(num, buffer, 10);
	return fputsn(fd, buffer, digits) + negative;
}

static int print_oct(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 8);
	return fputsn(fd, buffer, digits);
}

static int print_bin(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 2);
	return fputsn(fd, buffer, digits);
}

static int num_to_base(unsigned int value, char * buffer, unsigned int base) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do { 
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

/*****************************************************/

static int read_arg(int fd, va_list ap, char option);
static int read_dec(int fd, int * ptr);

int getchar() {
	return fgetc(STDIN);
}

int fgetc(int fd) {
	char c;
	read(fd, &c, 1);
	return c;
}

char * gets(char * str, int n) {
	return fgets(STDIN, str, n);
}

char * fgets(int fd, char * str, int n) {
	char c;
	int read = 0;

	while ((c = fgetc(fd)) != EOF && c != '\n' && read < n) {
		if (c > 0) {
			str[read++] = c;
			fputc(STDOUT, c);
		}
	}

	if (read == 0 && c == EOF) {
		return NULL;
	}

	str[read] = 0;
	return str;
}

int scanf(const char * fmt, ...) {
	va_list ap;
	int read;

	va_start(ap, fmt);
	read = vfscanf(STDIN, fmt, ap);
	va_end(ap);

	return read;
}

int fscanf(int fd, const char * fmt, ...) {
	va_list ap;
	int read;

	va_start(ap, fmt);
	read = vfscanf(fd, fmt, ap);
	va_end(ap);

	return read;
}

//TODO
int sscanf(const char * str, const char * fmt, ...) {
	return -1;
}

//TODO
int vfscanf(int fd, const char * fmt, va_list ap) {
	int read = 0;
	char c;

	while ((c = *fmt++) != 0) {
		if (c == '%') {
			char option = *(fmt++);
			if (read_arg(fd, ap, option) == 0) {
				return read;
			}
			read++;
		}
		else {
			fputc(fd, c);
		}
	}

	return read;
}

#define MAX_SIZE 64

//TODO
static int read_arg(int fd, va_list ap, char option) {
	int read = 0;
	char * arg;

	switch (option) {
		case 's':
			read = (fgets(fd, va_arg(ap, char *), MAX_SIZE) == NULL ? 0 : 1);
			break;
		case 'd':
			read = read_dec(fd, va_arg(ap, int *));
			break;
		case 'c':
			read = 1;
			arg = va_arg(ap, char *);
			(*arg) = fgetc(fd);
			fputc(STDOUT, *arg);
			break;
	}

	return read;
}

//TODO esta re buguete
static int read_dec(int fd, int * ptr) {
	int number = 0;
	int digits = 0;
	char c;

	while (digits < 9 && (c = fgetc(fd)) != '\n') {
		fputc(fd, c);
		if (c < '0' || c > '9') {
			return 0;
		}
		number = number * 10 + c - '0';
		digits++;
	}

	*ptr = number;
	return 1;
}