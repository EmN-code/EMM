#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <xtimer.h>

void periodic_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Periodic callback executed.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_periodic_event_t timer_event;

    // Initialize the periodic event timer with the callback
    evtimer_init(&evtimer, periodic_callback);

    // Configure the timer to execute every 1 second (1,000,000 microseconds)
    evtimer_periodic_add(&evtimer, &timer_event, PERIODIC, 1000000U, NULL);

    // Start the timer
    evtimer_periodic_start(&evtimer);

    // Let the main function pause
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}