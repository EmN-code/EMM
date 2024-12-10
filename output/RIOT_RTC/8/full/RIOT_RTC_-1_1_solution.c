#include <stdio.h>
#include "xtimer.h"
#include "rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg; // to avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t cb = rtc_alarm_callback;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, where 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;         // Daylight saving time flag

    while (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time, retrying...\n");
        xtimer_sleep(1);
    }

    // Setup alarm for 5 seconds later
    time.tm_sec += 5;
    mktime(&time); // Normalize the tm structure

    while (rtc_set_alarm(&time, cb, NULL) != 0) {
        printf("Failed to set RTC alarm, retrying...\n");
        xtimer_sleep(1);
    }

    printf("RTC Alarm set for 5 seconds from June 1, 2024.\n");
    return 0;
}