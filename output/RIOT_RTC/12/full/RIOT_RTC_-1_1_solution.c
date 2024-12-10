#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <time.h>
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    int rc;
    struct tm time;
    rtc_alarm_cb_t alarm_cb = rtc_alarm_callback;

    // Set the RTC time to 2024-06-01 00:00:00
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // 0-based, so June is month 5
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    while (1) {
        rc = rtc_set_time(&time);
        if (rc != 0) {
            printf("RTC set time failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("RTC time set successfully\n");
        break;
    }

    // Calculate alarm time, 5 seconds from the set time
    time.tm_sec += 5;
    mktime(&time);

    while (1) {
        rc = rtc_set_alarm(&time, alarm_cb, NULL);
        if (rc != 0) {
            printf("RTC set alarm failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("RTC alarm set successfully\n");
        break;
    }

    return 0;
}