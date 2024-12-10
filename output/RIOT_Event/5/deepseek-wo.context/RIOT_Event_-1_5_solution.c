
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t queue;
    event_periodic_callback_t periodic_event;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &queue, periodic_callback, NULL);

    // Set the interval to 1 second (1,000,000 microseconds)
    event_periodic_callback_set_interval(&periodic_event, 1000000);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&queue);
    }

    return 0;
}
