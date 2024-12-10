#include <time.h>  // Provides the tm structure and time functions
#include <stdio.h>  // For printf function
#include "rtc_alarm_cb_t.h"  // Assuming a custom header for RTC

// Callback function that will be called on RTC alarm trigger
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    // Create a tm structure for the desired date and time
    struct tm time_info;
    time_info.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time_info.tm_mon = 6 - 1;         // tm_mon is 0-based
    time_info.tm_mday = 1;            // Day of the month
    time_info.tm_hour = 0;            // Midnight
    time_info.tm_min = 0;             // 0 minute
    time_info.tm_sec = 0;             // 0 second

    // Set the RTC time to this date
    rtc_set_time(&time_info);

    // Calculate new time for the alarm (5 seconds added)
    time_t raw_time = mktime(&time_info);
    raw_time += 5;
    struct tm *alarm_time = localtime(&raw_time);

    // Set the RTC alarm
    rtc_set_alarm(alarm_time, alarm_callback, NULL);

    while (1) {
        // Loop indefinitely, let RTC interrupt handle the alarm
    }

    return 0;
}