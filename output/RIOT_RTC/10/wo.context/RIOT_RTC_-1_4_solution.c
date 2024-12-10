#include <stdio.h>
#include "time.h"

// Callback function for the alarm
void rtc_alarm_callback(void *arg) {
    (void)arg;  // to avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    rtc_alarm_cb_t alarm_cb;
    int result;

    // Set the date to June 1, 2024
    time_info.tm_year = 2024 - 1900; // Years since 1900
    time_info.tm_mon = 5;  // 0-11 (months since January, so 5 for June)
    time_info.tm_mday = 1; // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Set RTC time
    if ((result = rtc_set_time(&time_info)) != 1) {
        printf("RTC time setting failed: %d\n", result);
        return -1;
    }

    // Set an alarm 5 seconds from now
    time_info.tm_sec += 5;
    mktime(&time_info);  // normalize time structure

    if ((result = rtc_set_alarm(&time_info, rtc_alarm_callback, NULL)) != 1) {
        printf("RTC alarm setting failed: %d\n", result);
        return -1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    // Wait for the alarm to trigger
    while (1);

    return 0;
}