
#include <stdio.h>
#include "thread.h"
#include "event.h"
#include "ztimer.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

void periodic_callback(event_t *event) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event callback
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback);

    // Set the interval for the periodic event to 1 second
    event_periodic_callback_set_interval(&periodic_event, 1000);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
