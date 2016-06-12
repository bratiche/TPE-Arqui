#include <lib.h>
#include <video.h>
#include <io.h>

#define ROWS 25
#define COLS 80
#define SCREENSIZE ROWS * COLS * 2

#define out_of_bounds(r, c) (r < 0 || r >= ROWS || c < 0 || c >= COLS)

static void scroll(void);
static void delete(void);
static void update_cursor(void);

char * video = (char *) 0xB8000;
unsigned int current_loc = 0;

void fill(char ch, char attr) {
	unsigned int i = 0;

	while (i < SCREENSIZE) {
		video[i++] = ch;
		video[i++] = attr;
	}
}

void clear(void) {
	int i = 0;
	fill(' ', LIGHT_GREY);
	current_loc = 0;
}

int putchar_at(char ch, char attr, int r, int c) {
	if(out_of_bounds(r, c)) {
		return INDEX_OUT_OF_BOUNDS_ERROR;
	}
	video[(c + r * COLS) * 2] = ch;
	video[(c + r * COLS) * 2 + 1] = attr;

	return 0;
}

int puts_at(char *str, char attr, int r, int c) {
	if(out_of_bounds(r, c)) {
		return INDEX_OUT_OF_BOUNDS_ERROR;
	}

	while(*str) {
		if(*str == '\n') {
			if(++r >= ROWS) {
				return INDEX_OUT_OF_BOUNDS_ERROR;
			}
			c = 0;
			str++;
			continue;
		}
		if(c == COLS) {
			if(++r >= ROWS) {
				return INDEX_OUT_OF_BOUNDS_ERROR;
			}
			c = 0;
		}
		putchar_at(*str++, attr, r, c++);
	}

	return 0;
}

int putnumber_at(int n, char attr, int r, int c) {
	int digits = 1;
	int number, i;
	char ch;

	if(out_of_bounds(r, c)) {
		return INDEX_OUT_OF_BOUNDS_ERROR;
	}
	if(n < 0) {
		putchar_at('-', attr, r, c++);
		n *= -1;
	}

	number = n;

	while((number /= 10) > 0) {
		digits++;
	}

	while(digits--) {
		number = n;
		i = digits;
		while(i--) {
			number /= 10;
		}

		if(c == COLS) {
			if(++r >= ROWS) {
				return INDEX_OUT_OF_BOUNDS_ERROR;
			}
			c = 0;
		}
		ch = number % 10 + '0';
		putchar_at(ch, attr, r, c++);
	}

	return 0;
}

void putnumber(int n, char attr) {
	int digits = 1;
	int number, i;
	char ch;

	if(n < 0) {
		putchar('-', attr);
		n *= -1;
	}

	number = n;

	while((number /= 10) > 0) {
		digits++;
	}

	while(digits--) {
		number = n;
		i = digits;
		while(i--) {
			number /= 10;
		}

		ch = number % 10 + '0';
		putchar(ch, attr);
	}
}

void putchar(char ch, char attr) {

	switch(ch) {
		case '\n':
			current_loc = current_loc + (COLS * 2 - current_loc % (COLS * 2));
			break;
		case '\b':
			if (current_loc > 0) {
				delete();
			}
			break;
		default:
			video[current_loc++] = ch;
			video[current_loc++] = attr;
			break;
	}

	if (current_loc >= SCREENSIZE) {
		scroll();
	}

	/* Update cursor position*/
	update_cursor();
}

void puts(char *str, char attr) {
	while(*str) {
		putchar(*str++, attr);
	}
}

void update_cursor(void) {
   	write_port(0x3D4, 14);
    write_port(0x3D5, (current_loc / 2) >> 8);
    write_port(0x3D4, 15);
    write_port(0x3D5, current_loc / 2);
}

void delete(void) {
	if (current_loc == 0) {
		return;
	}

	// check new line
	if ((current_loc / 2) % COLS == 0 && video[current_loc - 2] == ' ') {
		while (video[current_loc - 2] == ' ') {
			current_loc -= 2;
		}
	}
	else {
		video[--current_loc] = LIGHT_GREY;
		video[--current_loc] = ' ';
	}
}

void scroll(void) {
	memcpy((void *)video, (void *)(video + COLS * 2), SCREENSIZE - COLS * 2);
	memsetw((void *)(video + SCREENSIZE - COLS * 2), ' ' | (BLACK << 8), COLS);
	current_loc -= COLS * 2;
}