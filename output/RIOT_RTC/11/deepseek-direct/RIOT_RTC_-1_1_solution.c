
#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

#define ALARM_DELAY_SEC (5U)

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    rtc_init();

    /* Set the date to June 1, 2024 */
    struct tm time = {
        .tm_year = 2024 - 1900, /* years since 1900 */
        .tm_mon = 5,            /* months since January (0-11) */
        .tm_mday = 1,           /* day of the month (1-31) */
        .tm_hour = 0,           /* hours since midnight (0-23) */
        .tm_min = 0,            /* minutes after the hour (0-59) */
        .tm_sec = 0             /* seconds after the minute (0-59) */
    };

    rtc_set_time(&time);

    /* Set an alarm to trigger after 5 seconds */
    struct tm alarm_time = time;
    alarm_time.tm_sec += ALARM_DELAY_SEC;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    /* Sleep to allow the alarm to trigger */
    xtimer_sleep(ALARM_DELAY_SEC + 1);

    return 0;
}
