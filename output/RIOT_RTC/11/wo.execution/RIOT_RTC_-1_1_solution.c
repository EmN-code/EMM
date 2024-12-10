#include <stdio.h>
#include <time.h>
#include "rtc_set_alarm.h"  // Assuming this is the RTC header file you should use

// Callback function for the alarm
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC to default date/time
    struct tm current_time = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 5,             // Month, where 0 = January
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Assuming rtc_init() initializes the RTC with the current_time
    rtc_init(&current_time);

    // Set alarm to trigger after 5 seconds
    struct tm alarm_time = current_time;
    alarm_time.tm_sec += 5;

    // Register the alarm with the callback
    // rtc_set_alarm is assumed to be the function that sets the RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main application loop
    while (1) {
        // Simulate doing other application tasks
    }
    return 0;
}