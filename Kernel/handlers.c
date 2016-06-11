#include <types.h>
#include <keyboard.h>
#include <video.h>
#include <stdint.h>

void timer_handler();

void irqDispatcher(uint32_t irq) {	
	switch(irq) {
		case 0:
			timer_handler();
			break;
		case 1:
			keyboard_handler();
			break;		
	}
	return;
}

void timer_handler() {
	//timer tasks
}