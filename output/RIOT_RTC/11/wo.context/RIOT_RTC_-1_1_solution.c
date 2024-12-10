#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the current date and time
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC with the specified time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
    }

    // Set an alarm for 5 seconds later
    alarm.time.tm_year = time.tm_year;
    alarm.time.tm_mon = time.tm_mon;
    alarm.time.tm_mday = time.tm_mday;
    alarm.time.tm_hour = time.tm_hour;
    alarm.time.tm_min = time.tm_min;
    alarm.time.tm_sec = time.tm_sec + 5;
    alarm.cb = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm.time, alarm.cb, alarm.arg) != 0) {
        printf("Failed to set RTC alarm\n");
    }

    while (1) {
        xtimer_sleep(1);  // Sleep to allow RTC to trigger the alarm
    }

    return 0;
}