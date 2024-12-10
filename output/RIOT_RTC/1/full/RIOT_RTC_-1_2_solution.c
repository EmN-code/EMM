#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

// RTC time is represented in tm structure
struct tm time;
struct tm alarm_time;

// Callback function to handle the alarm
void handle_alarm(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC. Assume rtc_init() returns 0 on success
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is 0 based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_time_set(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Setting alarm to trigger 5 seconds from now
    alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize time structure
    
    if (rtc_alarm_set(&alarm_time, handle_alarm) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    while (1) {
        // Wait for the alarm to trigger
    }

    return 0;
}