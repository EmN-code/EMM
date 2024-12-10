
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include <stdio.h>

#define ALARM_PIN GPIO_PIN(0, 18)

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
    gpio_set(ALARM_PIN);
}

int main(void)
{
    /* Initialize the GPIO pin for the alarm */
    gpio_init(ALARM_PIN, GPIO_OUT);
    gpio_clear(ALARM_PIN);

    /* Initialize the RTC */
    rtc_init();

    /* Set the date to June 1, 2024 */
    struct tm time = {
        .tm_year = 2024 - 1900, /* years since 1900 */
        .tm_mon = 5,            /* months since January - [0, 11] */
        .tm_mday = 1,           /* day of the month - [1, 31] */
        .tm_hour = 0,           /* hours since midnight - [0, 23] */
        .tm_min = 0,            /* minutes after the hour - [0, 59] */
        .tm_sec = 0             /* seconds after the minute - [0, 59] */
    };
    rtc_set_time(&time);

    /* Set an alarm after 5 seconds */
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    /* Main loop */
    while (1) {
        /* Do nothing */
    }

    return 0;
}
