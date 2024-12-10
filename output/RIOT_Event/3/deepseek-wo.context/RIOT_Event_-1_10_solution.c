
#include <stdio.h>  // Include stdio.h to resolve printf declaration error
#include "event/periodic.h"  // Include necessary headers for event handling
#include "ztimer.h"  // Include ztimer for time management
#include "thread.h"  // Include thread for thread management

#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic event with an interval of 1000 milliseconds (1 second)
    event_periodic_callback_start(&periodic_event, 1000);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);  // Use event_queue_dispatch instead of event_queue_init
    }

    return 0;
}
