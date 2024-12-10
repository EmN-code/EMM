#include <stdio.h>
#include "xtimer.h"
#include "rtc.h"

// callback function to print message when alarm is triggered
void alarm_callback(void *arg) {
    (void)arg; // to avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();
    rtc_poweron();

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Month, 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time.\n");
        return 1;
    }

    // Set the alarm to trigger after 5 seconds
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm.\n");
        return 1;
    }

    // Run an infinite loop to keep the main function running
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}