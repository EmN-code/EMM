#include <stdio.h>
#include "periph/rtc.h"  // Please ensure this header exists and is correctly named in your RIOT environment

// Callback function to be called when alarm triggers
void alarm_callback(void *arg) {
    (void)arg;  // Ignore unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return 1;
    }

    // Set the date - June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon  = 5,           // Months since January (0-11)
        .tm_mday = 1,           // Day of the month (1-31)
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    rtc_set_time(&time);

    // Set an alarm in 5 seconds
    rtc_alarm_t alarm;
    rtc_get_time(&time);  // Get current time
    time.tm_sec += 5;  // Add 5 seconds

    // Set the alarm time with rtc_alarm_t structure
    alarm.time.tm_year = time.tm_year;
    alarm.time.tm_mon  = time.tm_mon;
    alarm.time.tm_mday = time.tm_mday;
    alarm.time.tm_hour = time.tm_hour;
    alarm.time.tm_min  = time.tm_min;
    alarm.time.tm_sec  = time.tm_sec;
    alarm.callback = alarm_callback;  // Set the callback
    alarm.arg = NULL;

    // Set the RTC alarm
    if (rtc_set_alarm(&alarm) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    return 0;
}