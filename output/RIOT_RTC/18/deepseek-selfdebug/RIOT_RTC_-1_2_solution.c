
#include "xtimer.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "fmt.h"

#define ALARM_DELAY_SEC (5U)

void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    rtc_init();

    /* Set the date to June 1, 2024 */
    struct tm time = {
        .tm_year = 2024 - 1900, /* years since 1900 */
        .tm_mon = 5,            /* months since January (0-11) */
        .tm_mday = 1,           /* day of the month (1-31) */
        .tm_hour = 0,           /* hours since midnight (0-23) */
        .tm_min = 0,            /* minutes after the hour (0-59) */
        .tm_sec = 0             /* seconds after the minute (0-60) */
    };

    rtc_set_time(&time);

    /* Set an alarm to trigger after 5 seconds */
    xtimer_t alarm_timer;
    alarm_timer.callback = alarm_callback;
    alarm_timer.arg = NULL;
    xtimer_set(&alarm_timer, ALARM_DELAY_SEC * 1000000UL);

    return 0;
}
