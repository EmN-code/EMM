
#include <stdio.h>
#include "thread.h"
#include "event.h"
#include "ztimer.h"

#define EVENT_QUEUE_SIZE 16
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    /* Initialize the event queue */
    event_queue_init(&event_queue);

    /* Initialize the periodic event */
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL, US_PER_SEC);

    /* Start the periodic event */
    event_periodic_callback_start(&periodic_event);

    /* Main loop */
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
