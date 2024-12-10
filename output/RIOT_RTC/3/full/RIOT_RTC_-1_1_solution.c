#include <stdio.h>
#include <time.h>
#include "rtc.h"
#include "ztimer.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    time_info.tm_year = 2024 - 1900; // years since 1900
    time_info.tm_mon = 6 - 1; // months since January [0-11]
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    time_info.tm_isdst = -1; // no daylight saving time

    // Initialize the RTC
    rtc_init();
    rtc_set_time(&time_info);

    // Set alarm to trigger in 5 seconds
    ztimer_sleep(ZTIMER_MSEC, 5000);
    rtc_set_alarm(NULL, rtc_alarm_callback, NULL);

    while (1) {
        // Busy wait
    }

    return 0;
}