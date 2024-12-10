
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;
static evtimer_t evtimer;
static evtimer_periodic_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue and thread
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Initialize the periodic event
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;
    periodic_event.interval = 1 * US_PER_SEC; // 1 second interval

    // Add the periodic event to the event timer
    evtimer_add_periodic(&evtimer, &periodic_event);

    // Start the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
