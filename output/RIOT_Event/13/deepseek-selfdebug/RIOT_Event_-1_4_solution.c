
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Create a periodic event
    event_t periodic_event;
    periodic_event.handler = periodic_event_callback;
    periodic_event.arg = NULL;

    // Set the periodic event to trigger every second
    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)event_post;
    periodic_timer.arg = &periodic_event;
    xtimer_set(&periodic_timer, XTIMER_TICKS(1000000));

    // Dispatch events
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
