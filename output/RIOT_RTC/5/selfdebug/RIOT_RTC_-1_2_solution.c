#include <stdio.h>
#include <stdbool.h>
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set current date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month, 0-11
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;            // 00:00:00 AM
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;          // Daylight saving not applicable

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Schedule alarm to trigger in 5 seconds
    struct tm alarm_time;
    alarm_time = time;  // Start from current date
    alarm_time.tm_sec += 5;  // Add 5 seconds
    mktime(&alarm_time);  // Normalize the time structure

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set. Date: %04d-%02d-%02d %02d:%02d:%02d\n",
           time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
           time.tm_hour, time.tm_min, time.tm_sec);

    // Keep the main thread running
    while (1) {}

    return 0;
}