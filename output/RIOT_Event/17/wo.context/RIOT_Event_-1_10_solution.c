#include <ztimer.h>
#include <stdio.h>

#define US_PER_SEC (1000000U) // Define microseconds per second if not defined already

// Periodic callback function
void event_periodic(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Event periodic callback executed\n");
}

int main(void) {
    // Declare and initialize a ztimer periodic timer
    ztimer_periodic_t timer;
    ztimer_t timer_id = ZTIMER_MSEC;
    ztimer_periodic_init(&timer, &timer_id, US_PER_SEC);

    // Start periodic callback every second
    while (1) {
        // Wake up periodically
        ztimer_periodic_wakeup(&timer, US_PER_SEC);
        event_periodic(NULL); // Call the event function
    }

    return 0;
}