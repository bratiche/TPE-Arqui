#include <keyboard.h>
#include <keyboard_map.h>
#include <video.h>
#include <io.h>

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT	0x64

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
		
		putchar(keyboard_map[keycode], LIGHT_GREY);
	}
}

