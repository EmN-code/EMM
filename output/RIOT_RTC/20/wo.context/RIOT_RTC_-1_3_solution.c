#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

// RTC alarm callback function
void alarm_callback(void *arg) {
    (void)arg;  // Avoid unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to init RTC\n");
        return 1;
    }

    // Set calendar date and time: June 1, 2024, 00:00:00
    struct tm time = {0};
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month since January (0-11)
    time.tm_mday = 1;           // Day of the month (1-31)

    rtc_set_time(&time);

    // Define alarm to trigger after 5 seconds
    struct tm alarm_time;
    alarm_time = time;
    alarm_time.tm_sec += 5;

    // Ensure the seconds are within valid range
    if (alarm_time.tm_sec >= 60) {
        alarm_time.tm_sec -= 60;
        alarm_time.tm_min += 1;
    }

    // Set the RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    printf("RTC alarm set for 5 seconds later.\n");

    return 0;
}