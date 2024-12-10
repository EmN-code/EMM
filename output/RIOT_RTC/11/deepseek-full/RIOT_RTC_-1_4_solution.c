
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define ALARM_DELAY_SEC 5

static volatile bool alarm_triggered = false;

void alarm_callback(void *arg)
{
    (void)arg;
    alarm_triggered = true;
}

int main(void)
{
    xtimer_t alarm_timer;

    puts("RTC Alarm Example");

    // Initialize the alarm timer
    xtimer_init();

    // Set the alarm to trigger after 5 seconds
    xtimer_set(&alarm_timer, ALARM_DELAY_SEC * US_PER_SEC, alarm_callback, NULL);

    // Wait for the alarm to trigger
    while (!alarm_triggered) {
        xtimer_usleep(100 * US_PER_MS);
    }

    // Print the message when the alarm is triggered
    puts("Alarm triggered!");

    return 0;
}
