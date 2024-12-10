#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "thread.h"

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm alarm_time;
    alarm_time.tm_year = 2024 - 1900; // Year since 1900
    alarm_time.tm_mon = 6 - 1;        // Month, where 0 = January
    alarm_time.tm_mday = 1;          // Day of the month
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 5;

    time_t alarm_epoch = mktime(&alarm_time);

    time_t now = time(NULL);

    if (alarm_epoch > now) {
        xtimer_ticks32_t ticks = xtimer_frames(alarm_epoch - now);
        xtimer_t timer;
        timer.callback = alarm_callback;
        timer.arg = NULL;
        xtimer_set(&timer, ticks);
    } else {
        printf("The alarm time is set in the past!\n");
    }

    while (1) {
        thread_sleep();
    }

    return 0;
}