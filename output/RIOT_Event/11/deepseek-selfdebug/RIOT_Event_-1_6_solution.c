
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"  // Include the header for event_thread_init

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event thread
    event_queue_t queue;
    event_queue_init(&queue);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(INTERVAL_MS * US_PER_MS), periodic_callback, NULL);

    // Add the periodic event to the queue
    event_periodic_add(&queue, &periodic_event);

    // Start the event thread
    event_thread_init(&queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    // Main loop to keep the thread running
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
