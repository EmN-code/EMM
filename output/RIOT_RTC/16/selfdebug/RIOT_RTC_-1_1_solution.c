#include <stdio.h>
#include "rtc.h" // Include RTC functionalities

// Callback function for RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC Date and Time, and Scheduling Alarm\n");

    // Initialize RTC if necessary
    rtc_init();

    // Set initial date and time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 6 - 1,       // Month, 0 = January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Set the RTC current time
    rtc_set_time(&time);

    // Schedule alarm 5 seconds in future
    time.tm_sec += 5;
    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    while (1) {
        // Placeholder for other application logic
    }

    return 0;
}