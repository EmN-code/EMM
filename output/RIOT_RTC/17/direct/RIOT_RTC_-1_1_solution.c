#include <stdio.h>
#include "xtimer.h"
#include "ztimer.h"
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
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

    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    xtimer_sleep(5); // Wait for 5 seconds after setting the time

    rtc_get_time(&alarm_time);
    rtc_alarm_t alarm;
    alarm.time.tm_year = alarm_time.tm_year;
    alarm.time.tm_mon  = alarm_time.tm_mon;
    alarm.time.tm_mday = alarm_time.tm_mday;
    alarm.time.tm_hour = alarm_time.tm_hour;
    alarm.time.tm_min  = alarm_time.tm_min;
    alarm.time.tm_sec  = alarm_time.tm_sec + 5; // Set alarm for 5 seconds later
    alarm.callback = rtc_alarm_cb;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    // Wait indefinitely to catch the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}