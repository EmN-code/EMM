
#include <stdio.h>
#include <stdlib.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <periph/i2c.h>
#include <xtimer.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <stdbool.h>
#include <msg.h>
#include <periph/rtc.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define ALARM_DELAY_SEC (5)

static evtimer_msg_t alarm_timer;
static msg_t alarm_msg;

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time_struct = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 5,            // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-60)
    };
    rtc_set_time(&time_struct);

    // Initialize the event timer
    evtimer_init_msg(&alarm_timer);

    // Set up the alarm event
    evtimer_msg_event_t alarm_event;
    alarm_event.event.handler = alarm_callback;
    alarm_event.event.arg = NULL;

    // Calculate the alarm time
    timex_t now = timex_now();
    timex_t alarm_time = timex_add(now, timex_set(ALARM_DELAY_SEC, 0));

    // Add the alarm event to the timer
    evtimer_add_msg(&alarm_timer, &alarm_event, thread_getpid(), alarm_time);

    // Wait for the alarm to trigger
    msg_receive(&alarm_msg);

    return 0;
}
