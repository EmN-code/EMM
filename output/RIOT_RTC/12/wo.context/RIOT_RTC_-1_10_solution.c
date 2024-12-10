#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define ALARM_DURATION (5U * US_PER_SEC) // Alarm duration in microseconds

static void alarm_handler(void *arg) {
    (void)arg; // Unused variable
    puts("Alarm triggered!");
}

int main(void) {
    // Set the date to June 1, 2024 (assuming the use of a hypothetical rtc_set_date function)
    // rtc_set_date(2024, 6, 1);

    // Initialize the timer
    xtimer_ticks32_t ticks = xtimer_usec_from_ticks(ALARM_DURATION);
    xtimer_t alarm_timer;

    // Set the callback function and argument
    alarm_timer.callback = alarm_handler;
    alarm_timer.arg = NULL;

    // Set the timer
    xtimer_set(&alarm_timer, xtimer_usec_from_ticks(ALARM_DURATION));

    // Enter low-power mode to wait for the alarm (simulated here by sleeping)
    xtimer_sleep(10);

    return 0;
}