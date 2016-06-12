#include <keyboard.h>
#include <keyboard_map.h>
#include <video.h>
#include <io.h>

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT	0x64

#define BUFFER_SIZE 80		// una linea de pantalla 

char buffer[BUFFER_SIZE] = { -1 };
int current_pos = 0;		// si current_pos es cero, el buffer esta vacio (buffer[0] = -1 siempre)

void keyboard_handler(void) {
	unsigned char status;
	char keycode;

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if (keycode < 0) {
			return;
		}
		
		if (current_pos == BUFFER_SIZE - 1) {
			current_pos = 0;			
		}
		buffer[++current_pos] = keyboard_map[keycode];		

		//putchar(keyboard_map[keycode], LIGHT_GREY);
	}
}

/* Returns the ASCCI code of the last key pressed or -1 if the buffer is empty */
int get_key(void) {
	if (is_empty()) {
		return -1;
	}

	int c = buffer[--current_pos];

	return c;
}

/* Returns 0 if the buffer is empty */
int is_empty() {
	return current_pos == 0;
}