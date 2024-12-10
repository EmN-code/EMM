#include <stdio.h>
#include <stdlib.h>
#include "rtc.h"
#include "xtimer.h"

// Alarm callback function
void alarm_callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Setup RTC time to a specific date and time
    struct tm time;
    time.tm_year = 2024 - 1900;  // tm_year is the number of years since 1900
    time.tm_mon = 5;             // June (months since January, 0-11)
    time.tm_mday = 1;            // 1st
    time.tm_hour = 0;            // 0 hours
    time.tm_min = 0;             // 0 minutes
    time.tm_sec = 0;             // 0 seconds

    // Initialize RTC
    rtc_init();
    rtc_set_time(&time);

    // Set alarm to trigger 5 seconds from now
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // The system keeps running, using a sleep to simulate constant uptime
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}