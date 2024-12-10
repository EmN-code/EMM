#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    rtc_time_t rtc_time;

    // Set the RTC to June 1, 2024, 00:00:00
    time_info.tm_year = 2024 - 1900; // Year since 1900
    time_info.tm_mon = 6 - 1;        // Month, starting from 0
    time_info.tm_mday = 1;           // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Convert tm to rtc_time
    rtc_time.tm_sec = time_info.tm_sec;
    rtc_time.tm_min = time_info.tm_min;
    rtc_time.tm_hour = time_info.tm_hour;
    rtc_time.tm_mday = time_info.tm_mday;
    rtc_time.tm_mon = time_info.tm_mon;
    rtc_time.tm_year = time_info.tm_year;

    // Set RTC time
    if (rtc_set_time(&rtc_time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Check the current RTC time
    if (rtc_get_time(&rtc_time) != 0) {
        printf("Failed to get RTC time\n");
        return -1;
    }
    printf("Current RTC time set to: %d-%d-%d %d:%d:%d\n",
           rtc_time.tm_year + 1900, rtc_time.tm_mon + 1, rtc_time.tm_mday,
           rtc_time.tm_hour, rtc_time.tm_min, rtc_time.tm_sec);

    // Set the RTC alarm for 5 seconds from now
    rtc_time.tm_sec += 5;
    if (rtc_set_alarm(&rtc_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("Alarm set for 5 seconds from now\n");

    while (1) {
        // Simulating main loop
        xtimer_sleep(1);
    }

    return 0;
}