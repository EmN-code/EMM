
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic_callback.h>

// Define the callback function
void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic callback structure
    event_periodic_callback_t periodic_event;
    ztimer_clock_t *clock = ZTIMER_CLOCK_XTIMER;
    event_queue_t *queue = &event_queue_default;

    // Initialize the periodic callback
    event_periodic_callback_init(&periodic_event, clock, queue, periodic_callback);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&periodic_event, 1 * SEC_IN_USEC);

    // Start the periodic callback
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
