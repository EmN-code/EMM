
#include "xtimer.h"
#include "event/callback.h"
#include "thread.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t periodic_event;
    xtimer_t periodic_timer;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event.handler = periodic_event_callback;
    periodic_event.arg = NULL;

    // Initialize the periodic timer
    xtimer_init(&periodic_timer);

    // Set the periodic timer to trigger every second
    xtimer_set_periodic(&periodic_timer, XTIMER_TICKS(1000000), (xtimer_callback_t)event_post, &event_queue, &periodic_event);

    // Initialize the event thread
    event_thread_init(&event_queue, (char *)thread_stack_init(EVENT_THREAD_STACKSIZE), EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIORITY);

    // Start the event thread
    event_thread_start(&event_queue);

    return 0;
}
