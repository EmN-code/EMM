
#include "event/callback.h"
#include "event/periodic_callback.h"
#include "thread.h"
#include "ztimer.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_callback_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, NULL, EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIORITY);

    // Create and start the periodic callback event
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
