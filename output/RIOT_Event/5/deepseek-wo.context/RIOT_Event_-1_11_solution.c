
#include <stdio.h>
#include "thread.h"
#include "event.h"
#include "ztimer.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&periodic_event, 1000);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Main loop to process events
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
