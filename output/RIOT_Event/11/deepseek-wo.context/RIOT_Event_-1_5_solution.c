
#include <stdio.h>
#include "xtimer.h"
#include "event.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg; // Suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Create an event
    event_t event;
    event.handler = (event_handler_t)event_periodic_callback;

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger the event every second
    while (1) {
        xtimer_set_msg(&timer, TIMER_INTERVAL, &event, (kernel_pid_t)0);
        event_wait(&event);
    }

    return 0;
}
