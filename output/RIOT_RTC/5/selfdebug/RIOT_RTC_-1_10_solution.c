#include <stdio.h>
#include <stdlib.h>
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    rtc_time_t current_time;

    // Set the date to June 1, 2024
    time_info.tm_year = 2024 - 1900; // years since 1900
    time_info.tm_mon = 5; // months since January (0-11)
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Convert struct tm to rtc_time_t
    struct timeval tv;
    tv.tv_sec = mktime(&time_info);
    tv.tv_usec = 0;

    rtc_set_time(&tv);
    rtc_get_time(&current_time);
    printf("Current time set to: %s", ctime(&current_time.tv_sec));

    // Set the alarm to trigger after 5 seconds
    rtc_alarm_t alarm;
    rtc_get_time(&current_time);
    alarm.time.tm_sec = (current_time.tv_sec + 5) % 60;
    alarm.time.tm_min = (current_time.tv_sec / 60) % 60;
    alarm.time.tm_hour = (current_time.tv_sec / 3600) % 24;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm);

    while (1) {
        // Main loop does nothing, just waits for the alarm to trigger
    }

    return 0;
}