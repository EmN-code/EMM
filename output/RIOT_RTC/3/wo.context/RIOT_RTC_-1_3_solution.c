#include "ztimer.h"
#include "event/periodic.h"
#include "periph/rtc.h"
#include <stdio.h>

// RTC alarm callback
void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 6 - 1,        // 0-based month
        .tm_mday = 1,           // day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set the RTC date/time
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Setting RTC time failed!\n");
        return 1;
    }

    // Set an alarm for 5 seconds from now
    rtc_alarm_t alarm;
    alarm.tm = timeinfo;
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Delay before alarm

    if (rtc_set_alarm(&alarm, rtc_alarm_cb, NULL) != 0) {
        printf("Setting alarm failed!\n");
        return 1;
    }

    // Keeping the program running to wait for the alarm
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}