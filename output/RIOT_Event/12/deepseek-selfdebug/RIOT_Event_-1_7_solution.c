
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
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    // Create a periodic event
    xtimer_ticks32_t interval = xtimer_sec2ticks(1);
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, interval, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the thread running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
