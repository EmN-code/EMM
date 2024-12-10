#include <stdio.h>
#include "periph/rtc.h"
#include "xtimer.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is months since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    xtime_now_t now;
    rtc_get_time(&now);

    // Schedule alarm to be triggered 5 seconds later
    now += 5;
    rtc_set_alarm(now, rtc_alarm_cb, NULL);

    printf("Alarm set for 5 seconds from now\n");
    
    // Usually, you would enter a low power mode here or
    // continue running other tasks until the alarm triggers

    return 0;
}