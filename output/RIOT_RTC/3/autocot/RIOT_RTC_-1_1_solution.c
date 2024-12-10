#include <stdio.h>
#include <rtc.h>

// Callback function to handle RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,           // June
        .tm_mday = 1,           // 1st
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Initialize the RTC with the current date
    rtc_init();
    rtc_set_time(&time);

    // Set the RTC alarm to trigger after 5 seconds
    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    // Adjust the time struct to add 5 seconds for the alarm
    time.tm_sec += 5;
    mktime(&time); // Normalize time structure

    // Set the updated time with the alarm offset
    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    // Start the RTC
    rtc_start();

    // The program will continue running, waiting for the alarm to trigger
    while(1) {}

    return 0;
}