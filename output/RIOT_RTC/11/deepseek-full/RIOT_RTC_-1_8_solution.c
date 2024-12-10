
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <periph/rtc.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <stdbool.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define ALARM_DELAY_SECONDS 5

static volatile bool alarm_triggered = false;

void alarm_callback(void *arg)
{
    (void)arg;
    alarm_triggered = true;
}

int main(void)
{
    puts("RTC alarm example");

    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 5,            // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-59)
    };
    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    xtimer_t alarm_timer;
    xtimer_set(&alarm_timer, (ALARM_DELAY_SECONDS * US_PER_SEC), alarm_callback, NULL);

    // Wait for the alarm to trigger
    while (!alarm_triggered) {
        xtimer_usleep(100000); // Sleep for 100ms
    }

    // Print the message when the alarm is triggered
    puts("Alarm triggered!");

    return 0;
}
