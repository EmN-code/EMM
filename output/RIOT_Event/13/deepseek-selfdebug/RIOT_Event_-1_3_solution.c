
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"

#define EVENT_QUEUE_SIZE (8)
#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_queue_t event_queue;
static xtimer_t timer;

void event_periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic event triggered!");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_timeout_periodic_t periodic_event;
    event_timeout_periodic_init(&periodic_event, &event_queue, event_periodic_callback, NULL);

    // Start the periodic event with a period of 1 second
    xtimer_periodic_wakeup(&timer, &periodic_event, 1 * US_PER_SEC);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
