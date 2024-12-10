#include <stdio.h>
#include <errno.h>

#include "xtimer.h"
#include "periph/rtc.h"

void handle_alarm(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        fprintf(stderr, "RTC initialization failed\n");
        return -1;
    }

    // Define the date to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // Year - 1900
    time.tm_mon = 5; // Month, where 0 = jan
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        fprintf(stderr, "RTC set time failed\n");
        return -1;
    }

    // Set the alarm for 5 seconds from now
    time_t now = rtc_get_time();
    time_t alarm_time = now + 5;

    if (rtc_set_alarm(alarm_time, handle_alarm) != 0) {
        fprintf(stderr, "RTC set alarm failed\n");
        return -1;
    }

    // Wait for alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}