#include <stdio.h>
#include <time.h>
#include "timex.h"
#include "ztimer.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time;
    date_time.tm_year = 2024 - 1900; // Year since 1900
    date_time.tm_mon = 5;            // 0-based month (June)
    date_time.tm_mday = 1;           // Day of the month
    date_time.tm_hour = 0;
    date_time.tm_min = 0;
    date_time.tm_sec = 0;

    rtc_set_time(&date_time); // Set the RTC time to specified date and time

    struct tm alarm_time = date_time;
    alarm_time.tm_sec += 5; // Set alarm for 5 seconds later

    rtc_set_alarm(&alarm_time, alarm_callback, NULL); // Set alarm

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep to simulate periodic work
    }

    return 0;
}