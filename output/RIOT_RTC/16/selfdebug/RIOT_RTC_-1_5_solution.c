#include "xtimer.h"
#include "ztimer.h"
#include <stdio.h>

#define ALARM_SEC 5

void alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    /* Initialize RTC module if needed */
    struct tm time;

    time.tm_year = 2024 - 1900;  /* Year since 1900 */
    time.tm_mon = 5;             /* Month, where 0 = January */
    time.tm_mday = 1;            /* Day of the month */
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time((struct tm*) &time);

    /* calculate future time */
    time_t future_time = mktime(&time) + ALARM_SEC;
    struct tm *alarm_time = gmtime(&future_time);

    rtc_set_alarm(alarm_time, alarm_callback);

    while (1) {
        ;   /* Sleep, wait for the alarm */
    }

    return 0;
}