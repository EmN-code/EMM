#include <thread.h>
#include <stdio.h>
#include <rtc.h>

static void alarm_cb(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1, 
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_init();
    rtc_set_time(&time);

    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_cb, NULL);

    while (1) {
        thread_sleep();
    }

    return 0;
}