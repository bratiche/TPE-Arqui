#include <rtc.h>
#include <io.h>

#define RTC_OUT	0x70
#define RTC_IN	0x71

char * _time_ = "hh:mm:ss";

int get_time(char mode) {
 	write_port(RTC_OUT, mode);
 	return read_port(RTC_IN);
}

char * time(void) {
	int seconds = get_time(SECONDS);
	int minutes = get_time(MINUTES);
	int hours = get_time(HOURS);

	seconds = (0xF & seconds) + (10 * (seconds >> 4))+ (100 * (seconds >> 8))+ (1000 * (seconds >> 12));
	seconds = seconds % 60;

	minutes = (0xF & minutes) + (10 * (minutes >> 4)) + (100 * (minutes >> 8)) + (1000 * (minutes >> 12));
	minutes = minutes % 60;

	hours = (0xF & hours) + (10 * (hours >> 4)) + (100 * (hours >> 8)) + (1000 * (hours >> 12));
	hours = hours % 24;
	
	_time_[0] = ((hours / 10) % 10) + '0';
	_time_[1] = (hours % 10) + '0';
	_time_[3] = ((minutes / 10) % 10) + '0';
	_time_[4] = (minutes % 10) + '0';
	_time_[6] = ((seconds / 10) % 10) + '0';
	_time_[7] = (seconds % 10) + '0';

	return _time_;
}