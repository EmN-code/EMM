#include <time.h>            // For handling date and time
#include <stdio.h>           // For standard input-output functions
#include "rtc.h"              // RTC library from RIOT

// Callback function to be executed when the RTC alarm is triggered
void alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the RTC date and time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // Month (0-11, 0=January)
        .tm_mday = 1,           // Day of the month (1-31)
        .tm_hour = 0,           // Hour (0-23)
        .tm_min = 0,            // Minute (0-59)
        .tm_sec = 0             // Second (0-59)
    };
    rtc_set_time(&time);

    // Create an alarm that will trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize struct tm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Enter sleep mode or loop endlessly
    while (1) {
        // Sleep to save power, or just loop indefinitely for demo
    }

    return 0;
}