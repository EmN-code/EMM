#include <xtimer.h>
#include <stdio.h>
#include "rtc.h"

#define ALARM_DELAY_S 5

void rtc_alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    rtc_t dev = RTC_DEV(0); // Assuming RTC device 0

    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;  // 0-indexed, so June is 5
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(dev, &time) != RTC_OK) {
        puts("Failed to set time.");
        return 1;
    }

    // Set an alarm 5 seconds after the current time
    if (rtc_set_alarm_relative(dev, ALARM_DELAY_S, rtc_alarm_cb, NULL) != RTC_OK) {
        puts("Failed to set alarm.");
        return 1;
    }

    // This would normally sleep/idle the CPU, but we'll use it to keep the main thread alive
    while (1) {
        xtimer_sleep(10); // Keep the CPU in a low-power state
    }

    return 0;
}