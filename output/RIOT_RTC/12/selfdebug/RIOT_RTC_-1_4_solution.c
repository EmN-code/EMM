#include <stdio.h>
#include <time.h>     // Ensures we can use time-related functions
#include <periph/rtc.h>  // RTC library for RIOT

void rtc_alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC time structure
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,           // June (Months since January 0-11)
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Initialize RTC
    rtc_init();

    // Set time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
    }

    // Set alarm for 5 seconds later
    struct tm alarm_time = time;  // Start with current time

    // Update the seconds field
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the structure

    // Set alarm with the callback
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    return 0;
}