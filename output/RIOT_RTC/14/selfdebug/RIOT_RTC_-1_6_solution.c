#include <stdio.h>
#include <errno.h>
#include "timex.h"
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the current date to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5; // June (months since January)
    time.tm_mday = 1; // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set alarm 5 seconds from now
    alarm.time.tm_year = time.tm_year;
    alarm.time.tm_mon = time.tm_mon;
    alarm.time.tm_mday = time.tm_mday;
    alarm.time.tm_hour = time.tm_hour;
    alarm.time.tm_min = time.tm_min;
    alarm.time.tm_sec = time.tm_sec + 5; // 5 seconds from current time
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Sleep or wait indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}