#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time, alarm_time;
    alarm_t alarm;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set time: June 1, 2024, 00:00:00
    time.tm_year = 2024;
    time.tm_mon = 5;   // Months are 0-based in struct tm
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time!\n");
        return 1;
    }

    // Set alarm to trigger after 5 seconds
    alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_alarm_set(&alarm, &alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
        return 1;
    }

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}