#include <stdio.h>
#include <syscalls.h>
#include <stdarg.h>

static int print_arg(int fd, va_list ap, char option);
static int print_hex(int fd, int hex);
static int print_dec(int fd, int dec);
static int print_oct(int fd, int oct);
static int print_bin(int fd, int bin);
static int num_to_base(int value, char * buffer, int base);

void putchar(int c) {
	fputc(STDOUT, c);
}

void fputc(int fd, int c) {
	write(fd, &c, 1);
}

char getchar() {
	return fgetc(STDIN);
}

char fgetc(int fd) {
	char c;
	read(fd, &c, 1);
	return c;
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
			written += print_arg(STDOUT, ap, option);
		}
		else {
			fputc(STDOUT, c);
			written++;
		}
	}

	va_end(ap);
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
			fputc(fd, va_arg(ap, int));
			written++;
			break;
		default:
			return written;		// error de formato
	}

	return written;
}

static int print_hex(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 16);
	return fputsn(fd, buffer, digits);
}

static int print_dec(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 10);
	return fputsn(fd, buffer, digits);
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

static int num_to_base(int value, char * buffer, int base) {
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