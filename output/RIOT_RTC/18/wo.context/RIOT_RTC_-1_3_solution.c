#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

static void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 5,             // June (0-indexed, so 5 is June)
        .tm_mday = 1,            // 1st
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != RTC_OK) {
        puts("Failed to set RTC time and date.");
        return 1;
    }

    time_t alarm_time = timegm(&time) + 5;  // 5 seconds later
    rtc_alarm_t alarm = { .time = alarm_time, .callback = alarm_callback, .arg = NULL };

    if (rtc_set_alarm(&alarm) != RTC_OK) {
        puts("Failed to set RTC alarm.");
        return 1;
    }

    puts("RTC time and alarm configured. Waiting for alarm...");
    while(1) {
        // Infinite loop to keep the program running
    }

    return 0;
}