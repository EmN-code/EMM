#include "periph/rtc.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        puts("RTC initialization failed");
        return 1;
    }

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon  = 5,            // 0-based month (5 for June)
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    rtc_alarm_t alarm;
    rtc_get_time(&alarm.time);
    alarm.time.tm_sec += 5;

    rtc_set_alarm(&alarm, alarm_callback);

    printf("RTC alarm set for 5 seconds from now...\n");

    // Main loop does nothing, waiting for interrupts
    while (1) {}

    return 0;
}