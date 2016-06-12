#ifndef RTC_H_
#define RTC_H_

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 	0x04

/*
 00  RTC seconds
 01  RTC seconds alarm
 02  RTC minutes
 03  RTC minutes alarm
 04  RTC hours
 05  RTC hours alarm
 06  RTC day of week
 07  RTC day of month
 08  RTC month
 09  RTC year
 0A  RTC Status register A:
*/
 
char * time(void);

int get_time(char mode);


#endif