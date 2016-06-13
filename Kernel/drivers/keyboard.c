#include <keyboard.h>
#include <keyboard_map.h>
#include <video.h>
#include <io.h>

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT	0x64

#define BUFFER_SIZE 1			// por que no anda si pongo un buffer mas grande??
#define EMPTY 0xFF		//255

static void key_pressed(char keycode);
static void print_buffer(void);

static unsigned char buffer[BUFFER_SIZE] = { EMPTY };
static int write_pos = 0;
static int read_pos = 0;

void keyboard_handler(void) {
	unsigned char status;
	char keycode;

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		key_pressed(keycode);
		//print_buffer();
	}
}

void print_buffer() {
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		putchar(buffer[i], RED);
	}
}

/* Strores the keys pressed in the buffer */
void key_pressed(char keycode) {

	if (keycode < 0) {
		return;
	}
 
	//TODO shift, caps lock, etc

	buffer[write_pos++] = keyboard_map[keycode];

	if (write_pos == BUFFER_SIZE) {
		write_pos = 0;
	}
}

/* Returns the ASCCI code of the last key pressed or -1 if the buffer is empty */
unsigned char get_key(void) {
	unsigned char key = buffer[read_pos];
	buffer[read_pos++] = EMPTY;

	if (read_pos == BUFFER_SIZE) {
		read_pos = 0;
	}

	return key;
}

int is_empty(void) {
	return (buffer[read_pos] == EMPTY);
}