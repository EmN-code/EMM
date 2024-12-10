
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include <stdio.h>
#include <time.h>

#define I2C_DEV         I2C_DEV(0)
#define RTC_ALARM_PIN   GPIO_PIN(PORT_A, 0)

void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    /* Initialize I2C for RTC communication */
    i2c_init(I2C_DEV);

    /* Initialize GPIO for alarm pin */
    gpio_init_int(RTC_ALARM_PIN, GPIO_IN, GPIO_RISING, alarm_callback, NULL);

    /* Initialize RTC */
    if (rtc_init() < 0) {
        puts("RTC initialization failed");
        return 1;
    }

    /* Set the date to June 1, 2024 */
    struct tm time_struct = {
        .tm_year = 2024 - 1900, /* years since 1900 */
        .tm_mon = 5,            /* months since January - [0, 11] */
        .tm_mday = 1,           /* day of the month - [1, 31] */
        .tm_hour = 0,           /* hours since midnight - [0, 23] */
        .tm_min = 0,            /* minutes after the hour - [0, 59] */
        .tm_sec = 0             /* seconds after the minute - [0, 59] */
    };

    if (rtc_set_time(&time_struct) < 0) {
        puts("Setting RTC time failed");
        return 1;
    }

    /* Set an alarm after 5 seconds */
    struct tm alarm_time = time_struct;
    alarm_time.tm_sec += 5;
    if (rtc_set_alarm(&alarm_time, NULL) < 0) {
        puts("Setting RTC alarm failed");
        return 1;
    }

    /* Main loop */
    while (1) {
        /* Sleep or do other tasks */
    }

    return 0;
}
