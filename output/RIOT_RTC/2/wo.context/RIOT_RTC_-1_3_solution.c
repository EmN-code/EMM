#include "rtc_set_alarm.h"
#include "rtc_alarm_cb.h"
#include "rtc.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm time_set;
    time_set.tm_year = 2024 - 1900; // year since 1900
    time_set.tm_mon = 6 - 1; // month from 0 to 11
    time_set.tm_mday = 1;
    time_set.tm_hour = 0;
    time_set.tm_min = 0;
    time_set.tm_sec = 0;
    time_set.tm_isdst = -1; // no daylight saving time
    
    // Initialize and set RTC time
    rtc_init();
    rtc_set_time(&time_set);

    // Set alarm to trigger after 5 seconds
    int alarm_offset = 5; // seconds
    struct tm alarm_time = time_set;
    alarm_time.tm_sec += alarm_offset; // adding 5 seconds to current time

    // Set the RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Simulate wait
    while (1) {
        // this simulates the execution until the alarm triggers
    }

    return 0;
}