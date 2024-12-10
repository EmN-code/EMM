#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

ztimer_t alarm_timer;

void alarm_callback(void *arg) {
    (void)arg; // Unused variable
    puts("Alarm triggered!");
}

int main(void) {
    printf("Setting date to June 1, 2024, and scheduling an alarm in 5 seconds.\n");

    // Initialize the timer
    ztimer_set(ZTIMER_MSEC, &alarm_timer, 5000, alarm_callback, NULL);

    // Wait for the alarm to trigger
    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}