#include "xtimer.h"
#include "rtc.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    (void)arg;  // Ignore the argument
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    rtc_alarm_cb_t rtc_cb = alarm_callback;

    // Set the current date to June 1, 2024
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 5;             // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;         // No daylight saving time

    while (1) {
        if (rtc_init() == -1) {
            printf("RTC initialization failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC initialized successfully\n");
            break;
        }
    }

    // Set the RTC time
    if (rtc_set_time(&time) == -1) {
        printf("Setting RTC time failed\n");
        return 1;
    }

    // Create a 5-second alarm from now
    xtimer_sleep(1); // Give it a slight delay before setting alarm
    alarm_time = time;  // Start with current time
    alarm_time.tm_sec += 5;  // Add 5 seconds

    if (rtc_set_alarm(&alarm_time, rtc_cb, NULL) == -1) {
        printf("Setting RTC alarm failed\n");
        return 1;
    }

    while (1) {
        // Main loop does nothing, awaiting the alarm
    }

    return 0;
}