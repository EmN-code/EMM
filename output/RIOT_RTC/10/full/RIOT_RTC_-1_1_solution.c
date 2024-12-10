#include <stdio.h>
#include "xtimer.h"
#include "rtc.h"

// Callback function for the RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_time_get(&time); // Retrieve current time to avoid uninitialized access

    // Setting the date to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5;            // June is month 5 (0-based)
    time.tm_mday = 1;
    
    if (rtc_time_set(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    rtc_set_alarm(&time, rtc_alarm_callback, NULL);
    
    while (1) {
        // Simulating an external operation that keeps the MCU on
        printf("Waiting for the alarm to trigger...\n");
        xtimer_sleep(1);
    }

    return 0;
}