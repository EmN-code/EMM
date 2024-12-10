#include <time.h>
#include <stdio.h>
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg)
{
    (void)arg; // unused parameter
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1; // months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    time_t epoch_time = mktime(&timeinfo);
    if (rtc_set_time(epoch_time) < 0) {
        printf("RTC set time failed\n");
        return 1;
    }

    if (rtc_set_alarm(epoch_time + 5, rtc_alarm_callback, NULL) < 0) {
        printf("RTC set alarm failed\n");
        return 1;
    }

    printf("RTC time and alarm set for June 1, 2024, with an alarm after 5 seconds.\n");

    while (1) {
        // Keep the program running to wait for the alarm
    }

    return 0;
}