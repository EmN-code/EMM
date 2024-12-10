#include "xtimer.h"
#include "timex.h"
#include "rtc.h"
#include <stdio.h>

/* Callback function for RTC alarm */
void rtc_alarm_callback(void *arg) {
    (void) arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 6 - 1,         // 0-based month
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,            // Hour
        .tm_min = 0,             // Minute
        .tm_sec = 0              // Second
    };
    
    // Initialize the RTC
    if (rtc_init() != 0) {
        puts("Failed to initialize RTC");
        return 1;
    }

    // Set the current date and time
    if (rtc_set_time(&time) != 0) {
        puts("Failed to set RTC time");
        return 1;
    }
    
    // Set an alarm 5 seconds from now
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    
    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        puts("Failed to set RTC alarm");
        return 1;
    }

    puts("RTC alarm set for 5 seconds from now");

    // Sleep indefinitely, waiting for the alarm
    while (1) {
        xtimer_sleep(10);  // Sleep in blocks of 10 seconds
    }

    return 0;
}