#include <stdio.h>
#include <time.h>
#include "rtc.h"

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    struct tm time_info = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 6 - 1,         // Months since January (0-11)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_init();

    // Set the initial date and time
    rtc_set_time(&time_info);

    // Create time structure for alarm (5 seconds later)
    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5;

    // Set rtc alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main loop
    while (1) {
        // Sleep to let the RTC alarm interrupt happen
        // In real-world scenario, use proper low-power sleep
    }
    return 0;
}