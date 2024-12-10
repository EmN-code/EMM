
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), periodic_callback, NULL);

    // Add the periodic event to the queue
    event_periodic_add(&event_queue, &periodic_event);

    // Keep the main thread running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
