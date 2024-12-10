#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "timex.h"
#include "periph/rtc.h"
#include <stdio.h>

#define ALARM_SECONDS 5  // Alarm time delay in seconds

void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC device
    if (rtc_init() < 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month 0-11
    time.tm_mday = 1;           // Day of the month 1-31
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Get the current time (ensure it's set correctly)
    if (rtc_get_time(&time) != 0) {
        printf("Failed to get RTC time\n");
        return 1;
    }

    // Calculate alarm time
    time_t current_time = mktime(&time);
    time_t alarm_time = current_time + ALARM_SECONDS;
    struct tm *alarm_tm = gmtime(&alarm_time);

    // Set the alarm
    if (rtc_set_alarm(alarm_tm, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Simulate doing other tasks
    while (1) {
        thread_sleep();
    }

    return 0;
}