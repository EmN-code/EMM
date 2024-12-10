#include <stdio.h>
#include <xtimer.h>
#include <pthread.h>
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    time.tm_year = 124; // Year since 1900, 2024 - 1900 = 124
    time.tm_mon = 5;    // 0-based, June = 5
    time.tm_mday = 1;   // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC with the current time
    if (rtc_init() != 0) {
        puts("Error initializing RTC.");
        return 1;
    }

    // Set RTC date to June 1, 2024
    if (rtc_set_time(&time) != 0) {
        puts("Error setting RTC time.");
        return 1;
    }

    // Set alarm for 5 seconds from now
    if (rtc_set_alarm_relative(5, alarm_callback, NULL) != 0) {
        puts("Error setting RTC alarm.");
        return 1;
    }

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(10); // sleep to keep program alive
    }

    return 0;
}