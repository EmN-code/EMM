#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month, where January is 0
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time.\n");
        return 1;
    }

    // Set alarm for 5 seconds from now
    time_t now = rtc_get_time();
    time_t alarm_time = now + 5; // 5 seconds later

    struct tm *alarm_tm = gmtime(&alarm_time);
    alarm.time = *alarm_tm;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    // Set the alarm
    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm.\n");
        return 1;
    }

    printf("RTC alarm set, waiting for it to trigger...\n");

    // Simulate doing other tasks
    while (1) {
        xtimer_sleep(UINT32_MAX);
    }

    return 0;
}