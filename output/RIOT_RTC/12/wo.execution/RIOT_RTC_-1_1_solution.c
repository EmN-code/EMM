#include <stdio.h>
#include <time.h>
#include "rtc.h"

int main(void) {
    struct tm set_time = {
        .tm_year = 2024 - 1900, // tm_year is years since 1900
        .tm_mon = 5,            // tm_mon is 0-based, so June is 5
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    rtc_init();
    printf("Initializing RTC\n");

    // Set current date and time
    rtc_set_time(&set_time);
    printf("Date set to June 1, 2024, 00:00:00\n");

    // Set alarm to trigger after 5 seconds
    struct tm alarm_time = set_time;
    alarm_time.tm_sec += 5;  // Add 5 seconds to the current time

    // Callback function when alarm is triggered
    void alarm_callback(void *arg) {
        (void)arg;  // Unused
        printf("Alarm triggered!\n");
    }

    // Set RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);
    printf("Alarm set to trigger in 5 seconds\n");

    // Keep the program running to wait for the alarm
    while (1) {
        // Sleep or do other processing
    }

    return 0;
}
