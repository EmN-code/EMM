// Description: This code sets up an RTC alarm on an ESP32 using RIOT. The date is set to June 1, 2024, and an alarm is triggered after 5 seconds. Upon alarm activation, it prints "Alarm triggered!".

// Imports
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

static void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_time_t time;
    rtc_alarm_t alarm;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set time to June 1, 2024, 00:00:00
    time.tm_year = 2024;
    time.tm_mon = 6;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm 5 seconds from now
    rtc_get_time(&time);
    time.tm_sec += 5;

    alarm.time = time;
    alarm.callback = alarm_callback;
    alarm.callback_arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm is set to trigger in 5 seconds\n");

    return 0;
}