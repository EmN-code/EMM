
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "kernel_defines.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic event with a 1 second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Main loop to process events
    while (1) {
        event_t *event = event_wait(&event_queue);
        if (event) {
            event_handler_t handler = event_get_handler(event);
            handler(event);
        }
    }

    return 0;
}
