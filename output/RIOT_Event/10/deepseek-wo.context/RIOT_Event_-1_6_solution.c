
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event callback triggered\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), periodic_event_cb, NULL);

    // Add the periodic event to the event queue
    event_periodic_add(&periodic_event);

    // Start the event thread
    event_thread_start();

    return 0;
}
