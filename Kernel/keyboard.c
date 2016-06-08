#include <keyboard.h>
#include <keyboard_map.h>
#include <video.h>

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT	0x64

#define DEL 14
#define LF 	28

void keyboard_handler(void) {
	unsigned char status;
	char keycode;

	/* write EOI (End Of Interrput acknowlegment) */
	//write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if (keycode < 0) {
			return;
		}
		
		//putnumber(keycode, LIGHT_GREY);

		putchar(keyboard_map[keycode], LIGHT_GREY);
	}
}

