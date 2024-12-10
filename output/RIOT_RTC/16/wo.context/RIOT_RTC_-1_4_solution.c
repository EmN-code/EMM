#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Set current date and time to 1st June 2024, 00:00:00
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set alarm 5 seconds after the current time
    alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_handler, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for %d:%d:%d\n", alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);

    while (1) {
        // Main loop does nothing; waiting for the alarm handler
        xtimer_sleep(1);
    }

    return 0;
}