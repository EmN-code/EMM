
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <periph/i2c.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <ztimer.h>

#define ALARM_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static volatile bool alarm_triggered = false;

void alarm_callback(void *arg)
{
    (void)arg;
    alarm_triggered = true;
    puts("Alarm triggered!");
}

int main(void)
{
    rtc_init();

    struct tm time_struct = {
        .tm_year = 2024 - 1900,  // years since 1900
        .tm_mon = 5,             // months since January (0-11)
        .tm_mday = 1,            // day of the month (1-31)
        .tm_hour = 0,            // hours since midnight (0-23)
        .tm_min = 0,             // minutes after the hour (0-59)
        .tm_sec = 0              // seconds after the minute (0-59)
    };

    rtc_set_time(&time_struct);

    xtimer_t alarm_timer;
    xtimer_set(&alarm_timer, ALARM_INTERVAL, alarm_callback, NULL);

    while (!alarm_triggered) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
