
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"

/* Define the event periodic callback structure */
event_periodic_callback_t periodic_event;

/* Define the event queue */
event_queue_t event_queue;

/* Define the callback function */
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    /* Initialize the event queue */
    event_queue_init(&event_queue);

    /* Initialize the periodic callback event */
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    /* Start the periodic callback event with an interval of 1 second */
    event_periodic_callback_start(&periodic_event, 1000000);

    /* Dispatch events from the event queue */
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
