#include <stdio.h>
#include <stdlib.h>
#include "rtc.h"
#include <xtimer.h>

/* RTC Alarm Callback Function */
void rtc_alarm_callback(void *arg) {
    (void)arg; // Avoid unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    /* Set the date to June 1, 2024 */
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;  // Month: June = 5 (0-indexed, i.e., January = 0)
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    /* Get the current RTC time */
    rtc_get_time(&time);

    /* Set alarm to trigger after 5 seconds */
    time.tm_sec += 5;
    mktime(&time); // Normalize time structure
    alarm.time = time;
    alarm.cb = rtc_alarm_callback;
    alarm.arg = NULL;

    /* Set the RTC alarm */
    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    /* Wait for the alarm */
    xtimer_sleep(10); // Sleep long enough to ensure alarm triggers

    return 0;
}