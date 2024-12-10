#include <stdio.h>
#include "rtc.h"
#include "xtimer.h"

static void alarm_callback(void *arg) {
    (void)arg;  // to silence unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,           // 0-based month (June)
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm to trigger in 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}