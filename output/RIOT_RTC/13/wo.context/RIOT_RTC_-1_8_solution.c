#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "rtc.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, from 0 (January) to 11 (December)
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);
    printf("Current RTC time set to: %s", ctime(&time.tv_sec));

    // Set the alarm for 5 seconds later
    alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the tm structure

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Placeholder to simulate a running environment waiting for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}