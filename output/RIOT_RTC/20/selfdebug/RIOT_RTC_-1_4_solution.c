#include <stdio.h>
#include <string.h>
#include <time.h>

#include "rtc.h"
#include "xtimer.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // 0-indexed months
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return -1;
    }

    // Set the alarm to trigger after 5 seconds
    xtimer_sleep(5);

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm.\n");
        return -1;
    }

    printf("RTC time and alarm set successfully.\n");
    return 0;
}