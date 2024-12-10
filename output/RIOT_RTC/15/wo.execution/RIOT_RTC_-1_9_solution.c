#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <event/callback.h>
#include <event/periodic.h>

#define ALARM_OFFSET_SEC   (5U)

static void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    struct tm time;
    rtc_t alarm;
    struct tm alarm_time;

    /* Set date to June 1, 2024 */
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();
    rtc_set_time(&time);

    /* Set an alarm for 5 seconds later */
    alarm_time = time;
    alarm_time.tm_sec += ALARM_OFFSET_SEC;
    mktime(&alarm_time); /* Normalize the time structure */

    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm_time, &alarm);

    while (1) {
        /* Simulate doing other things while waiting for the alarm */
        xtimer_sleep(1);
    }

    return 0;
}