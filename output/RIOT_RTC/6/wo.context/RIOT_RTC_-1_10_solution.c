#include <stdio.h>
#include <time.h>
#include "xtimer.h"

void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC and set the date to June 1, 2024
    struct tm rtc_time = {0};
    rtc_time.tm_year = 2024 - 1900; // years since 1900
    rtc_time.tm_mon = 5;  // June (0-based)
    rtc_time.tm_mday = 1; // 1st
    rtc_time.tm_hour = 0;
    rtc_time.tm_min = 0;
    rtc_time.tm_sec = 0;
    time_t rtc_timestamp = mktime(&rtc_time);

    // Set RTC to this time
    struct timeval tv = {.tv_sec = rtc_timestamp};
    settimeofday(&tv, NULL);

    // Set an alarm to trigger after 5 seconds
    xtimer_sleep(5);

    // Trigger the alarm callback
    rtc_alarm_callback(NULL);

    return 0;
}