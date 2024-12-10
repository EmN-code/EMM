
#include <stdio.h>
#include "xtimer.h"
#include "event.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event.handler = (event_handler_t)periodic_callback;
    periodic_event.next = NULL;

    // Start the periodic event
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_queue_add;
    timer.arg = &periodic_event;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Run the event loop
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
