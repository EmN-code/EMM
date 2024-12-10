
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static char event_thread_stack[THREAD_STACKSIZE_DEFAULT];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    // Create and start the periodic event
    event_periodic_callback_t periodic_cb_event;
    event_periodic_callback_create(&periodic_cb_event, &xtimer_clock, INTERVAL_MS, &event_queue, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_callback_start(&periodic_cb_event);

    // Main loop to keep the application running
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
