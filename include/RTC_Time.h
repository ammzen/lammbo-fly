#ifndef _RTC_TIME_H_
#define _RTC_TIME_H_
#include <time.h>

extern struct tm Time_ConvUnixToCalendar(time_t t);
extern time_t Time_ConvCalendarToUnix(struct tm t);
extern time_t Time_GetUnixTime(void);
extern struct tm Time_GetCalendarTime(void);
extern void Time_SetUnixTime(time_t);
extern void Time_SetCalendarTime(struct tm t);


void time_rest(void);
void show_time(void);




void RTC_Config(void);

extern time_t current_time;
extern struct tm time_now;
extern vu32 TimeDisplay;

extern volatile u8 key_time_rest;

#endif
