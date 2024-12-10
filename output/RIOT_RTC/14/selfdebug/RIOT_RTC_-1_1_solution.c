#include <stdio.h>
#include <time.h>
#include "rtc.h"

static void rtc_alarm_callback(void *arg)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    rtc_alarm_cb_t cb;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is 0-based
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return -1;
    }

    // Set the RTC time
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Error setting RTC time\n");
        return -1;
    }

    // Set alarm for 5 seconds from now
    timeinfo.tm_sec += 5;
    cb = rtc_alarm_callback;

    if (rtc_set_alarm(&timeinfo, cb, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return -1;
    }

    printf("RTC Alarm set for 5 seconds from now\n");

    // Loop forever, waiting for interrupts
    while (1) {}

    return 0;
}