
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
    printf("Periodic event callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Create a periodic event
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    event_periodic_t periodic_event;
    event_periodic_callback_init(&periodic_event, interval, periodic_event_callback, NULL);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
