#include <timer.h>
#include <video.h>
#include <rtc.h>

#define TICKS_PER_SECOND 18

unsigned int ticks = 0;

/* Called 18 times a second */
void timer_handler(void) {

	ticks++;
	if (ticks % TICKS_PER_SECOND == 0) {
		puts_at(time(), GREEN, 0, 72);
	}

}
