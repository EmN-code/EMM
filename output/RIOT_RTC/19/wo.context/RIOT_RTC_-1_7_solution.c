#include <xtimer.h>
#include <stdio.h>
#include <periph/rtc.h>

void rtc_alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    time.tm_year = 124;  // Year since 1900, so 2024 - 1900 = 124
    time.tm_mon = 5;     // Month, 0 = Jan, so June is 5
    time.tm_mday = 1;    // Day of the month
    time.tm_hour = 0;    // Midnight
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set alarm 5 seconds from now
    time.tm_sec += 5;

    if (rtc_set_alarm(&time, rtc_alarm_cb, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    while (1) {
        xtimer_sleep(1);  // Just doing nothing, waiting for the alarm
    }

    return 0;
}