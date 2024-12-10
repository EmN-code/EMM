#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/rtc.h"

// Callback function to be called when the alarm is triggered
void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;  // 0-based month, June
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Get the current time for setting the alarm
    struct tm now;
    if (rtc_get_time(&now) != 0) {
        printf("Failed to get current time\n");
        return 1;
    }

    // Set the alarm to trigger 5 seconds later
    now.tm_sec += 5;
    mktime(&now); // Normalize the time structure

    if (rtc_set_alarm(&now, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
