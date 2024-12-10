#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>

void rtc_alarm_callback(void *arg)
{
    (void)arg; // Cast to void to silence unused variable warning
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_time_t time;
    rtc_time_t alarm_time;

    // Set date to June 1, 2024, 00:00:00
    time.tm_year = 2024;
    time.tm_mon = 5; // Month is 0-indexed
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    while (rtc_init() != 0) {
        printf("RTC initialization failed! Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Wait for 1 second before retrying
        continue;
    }

    printf("RTC initialized successfully.\n");

    // Set the current time
    rtc_set_time(&time);

    // Set alarm for 5 seconds from now
    alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Wait indefinitely for the alarm callback
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}