
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_t event;
static event_queue_t event_queue;
static char event_thread_stack[EVENT_THREAD_STACKSIZE];

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Initialize the periodic event
    event_periodic_init(&event, event_periodic_callback, NULL, xtimer_ticks32(xtimer_s2ticks(1)));

    // Add the event to the event queue
    event_periodic_add(&event_queue, &event);

    // Start the event thread
    event_thread_start(&event_queue);

    return 0;
}
