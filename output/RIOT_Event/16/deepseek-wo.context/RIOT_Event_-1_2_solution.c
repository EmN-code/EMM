
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event
    event_init(&event, periodic_callback, NULL);

    // Start the periodic event
    while (1) {
        event_post(&event_queue, &event);
        xtimer_sleep(PERIODIC_INTERVAL);
    }

    return 0;
}
