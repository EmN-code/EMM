#include <stdio.h>
#include <time.h>
#include "rtc_alarm_cb_t.h"

static void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_alarm_cb_t alarm_cb = alarm_callback;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // years since 1900
    timeinfo.tm_mon = 6 - 1; // months since January
    timeinfo.tm_mday = 1; // day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC which is assumed to have rtc_init() called elsewhere

    // Set the alarm to trigger 5 seconds after setting the date
    timeinfo.tm_sec += 5; // add 5 seconds to current time

    if (rtc_set_alarm(&timeinfo, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Loop forever
    while (1);
    return 0;
}