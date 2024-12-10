
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "event/thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_t periodic_event;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), periodic_callback, NULL);

    // Add the periodic event to the queue
    event_periodic_add(&queue, &periodic_event);

    // Start the event thread
    event_thread_start(&queue);

    return 0;
}
