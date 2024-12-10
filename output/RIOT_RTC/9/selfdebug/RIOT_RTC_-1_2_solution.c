#include <stdio.h>
#include <xtimer.h>
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon  = 6 - 1,       // Months since January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    rtc_set_time(&time);

    // Plan triggering the alarm after 5 seconds
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    xtimer_sleep(5);

    // Set the alarm
    rtc_alarm_t alarm = {
        .alarm_time = alarm_time,
        .cb = rtc_alarm_cb,
        .arg = NULL
    };
    rtc_set_alarm(&alarm);

    return 0;
}