#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/rtc.h>

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,  // 0-11, so 5 means June
        .tm_mday = 1, // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_init();
    rtc_set_time(&time);

    // Add 5 seconds to the current time for the alarm
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Sleep to wait for the alarm
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}