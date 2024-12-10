
#include <stdio.h>
#include "thread.h"
#include "event.h"
#include "event/callback.h"
#include "event/periodic_callback.h"
#include "ztimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic callback event with an interval of 1 second
    event_periodic_callback_start(&periodic_event, 1000);

    // Process events
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
