#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "dht.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "assert.h"
#include "time_units.h"
#include "mutex.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

static void cb_alarm(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    rtc_t rtc_dev;
    rtc_time_t time = {
        .tm_year = 124,  // Year since 1900
        .tm_mon = 5,     // Months since January
        .tm_mday = 1,    // Day of the month
        .tm_hour = 0,    // Hours since midnight
        .tm_min = 0,     // Minutes after the hour
        .tm_sec = 0      // Seconds after the minute
    };
    rtc_alarm_t alarm = {
        .time = {
            .tm_year = 124,
            .tm_mon = 5,
            .tm_mday = 1,
            .tm_hour = 0,
            .tm_min = 0,
            .tm_sec = 5
        },
        .cb = cb_alarm,
        .arg = NULL
    };

    // Initialize RTC
    if (rtc_init(&rtc_dev) != 0) {
        puts("Error initializing RTC");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time(&rtc_dev, &time) != 0) {
        puts("Error setting time");
        return 1;
    }

    // Set RTC alarm
    if (rtc_set_alarm(&rtc_dev, &alarm) != 0) {
        puts("Error setting alarm");
        return 1;
    }

    while (1) {
        // Main loop can perform other tasks or enter low power mode
        xtimer_sleep(1);
    }

    return 0;
}