#include <stdio.h>
#include "rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg; // Suppress the unused parameter warning
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // 0-based month
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize the RTC time
    if (rtc_set_time(&time) != 0) {
        puts("Failed to set time.");
        return 1;
    }

    // Set the alarm 5 seconds from now
    time.tm_sec += 5;

    if (rtc_set_alarm(&time, rtc_alarm_callback, NULL) != 0) {
        puts("Failed to set alarm.");
        return 1;
    }

    return 0;
}