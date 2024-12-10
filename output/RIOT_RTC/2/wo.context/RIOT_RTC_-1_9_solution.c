#include <stdio.h>
#include <time.h>
#include "xtimer.h"

void rtc_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    time_t now;
    struct tm *alarm_time;

    printf("Setting RTC date to June 1, 2024\n");

    // Set the current date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1; // Months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1; // No daylight saving time

    now = mktime(&timeinfo);
    if (now == (time_t)(-1)) {
        printf("Failed to set RTC date!\n");
        return 1;
    }

    // Set the system time
    struct timeval set_time = { .tv_sec = now };
    settimeofday(&set_time, NULL);

    // Schedule alarm 5 seconds from now
    alarm_time = localtime(&now);
    alarm_time->tm_sec += 5;

    time_t alarm = mktime(alarm_time);
    time_t diff = alarm - now;

    printf("Setting RTC alarm for 5 seconds later\n");
    xtimer_set_msg(&xtimer, diff * 1000000UL, rtc_callback, NULL);

    // Main loop
    while(1) {
        // Placeholder for demonstrating a working loop
        xtimer_sleep(1);
    }

    return 0;
}