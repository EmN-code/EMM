#include <stdio.h>
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg)
{
    (void)arg;  // unused
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;

    // Set current time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1;        // tm_mon is months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    rtc_alarm_t alarm;

    // Set alarm for 5 seconds later
    alarm.time.tm_year = 2024 - 1900;
    alarm.time.tm_mon = 6 - 1;
    alarm.time.tm_mday = 1;
    alarm.time.tm_hour = 0;
    alarm.time.tm_min = 0;
    alarm.time.tm_sec = 5;  // 5 seconds after setting
    alarm.callback = rtc_alarm_cb;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Time is set and alarm is configured\n");

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}