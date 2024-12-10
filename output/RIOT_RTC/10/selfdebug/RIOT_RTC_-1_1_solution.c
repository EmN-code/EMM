#include <stdio.h>
#include "xtimer.h"
#include "rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;  // Month, where 0 = January
    time.tm_mday = 1;  // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC module
    rtc_init();

    // Set RTC time
    rtc_set_time(&time);

    // Set RTC alarm
    struct tm alarm_time = time;  // Copy the current time
    alarm_time.tm_sec += 5;  // Set alarm for 5 seconds later
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Keep main thread active to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
