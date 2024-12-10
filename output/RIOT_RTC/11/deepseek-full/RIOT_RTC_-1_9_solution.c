
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <mutex.h>
#include <time_units.h>

#define ALARM_DELAY_SECONDS 5

static volatile bool alarm_triggered = false;
static mutex_t alarm_mutex = MUTEX_INIT;

void alarm_callback(void *arg) {
    (void)arg;
    alarm_triggered = true;
    mutex_unlock(&alarm_mutex);
}

int main(void) {
    xtimer_t alarm_timer;

    // Initialize the alarm timer
    xtimer_init(&alarm_timer);

    // Set the alarm to trigger after 5 seconds
    xtimer_set(&alarm_timer, (timex_t){ALARM_DELAY_SECONDS, 0}, alarm_callback, NULL);

    // Lock the mutex to wait for the alarm
    mutex_lock(&alarm_mutex);

    // Wait until the alarm is triggered
    while (!alarm_triggered) {
        mutex_lock(&alarm_mutex);
    }

    // Print the message when the alarm is triggered
    puts("Alarm triggered!");

    return 0;
}
