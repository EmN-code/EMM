#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
    // Handle the alarm trigger event here
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    
    // Define the target date and time
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    if (rtc_init() != SUCCESS) {
        printf("Failed to initialize RTC\n");
        return FAILURE;
    }

    // Set RTC time
    if (rtc_set_time(&time) != SUCCESS) {
        printf("Failed to set time\n");
        return FAILURE;
    }

    // Define the alarm time, 5 seconds later
    alarm_time = time;
    alarm_time.tm_sec += 5;

    // Set RTC alarm
    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != SUCCESS) {
        printf("Failed to set alarm\n");
        return FAILURE;
    }

    printf("RTC set for %d-%d-%d %d:%d:%d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    printf("Alarm will trigger in 5 seconds\n");

    // Infinite loop to keep the program running for demo purposes
    while (1) {
        // In an actual implementation, the CPU can be put to sleep to save power
    }

    return SUCCESS;
}