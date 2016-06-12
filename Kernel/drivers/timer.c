#include <timer.h>
#include <video.h>

#define TICKS_PER_SECOND 18

unsigned int ticks = 0;

/* Called 18 times a second */
void timer_handler(void) {

	ticks++;
	if (ticks % TICKS_PER_SECOND == 0) {
		putchar_at('T', GREEN, 10, 10);
	}

}
