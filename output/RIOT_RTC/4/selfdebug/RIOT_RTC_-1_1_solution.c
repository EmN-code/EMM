#include "ztimer.h"
#include "thread.h"
#include "rtc.h"
#include <stdio.h>

#define ALARM_SEC 5

void rtc_alarm_callback(void *arg) {
    (void)arg;   // suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Initialize RTC
    rtc_init();

    // Set initial date and time: June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, starting from 0
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    // Set alarm to trigger after ALARM_SEC seconds
    time.tm_sec += ALARM_SEC;
    if (rtc_set_alarm(&time, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm.\n");
        return 1;
    }

    // Infinite loop to keep the main thread alive
    while (1) {
        // Yield to avoid busy waiting
        thread_yield();
    }

    return 0;
}