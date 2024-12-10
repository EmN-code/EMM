
#include "xtimer.h"
#include "event/callback.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_queue_t event_queue;
static xtimer_t periodic_timer;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, NULL, EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIORITY);

    // Set up the periodic timer
    periodic_timer.callback = event_periodic_callback;
    periodic_timer.arg = NULL;

    // Start the periodic timer with a 1-second interval
    xtimer_set_periodic(&periodic_timer, 1 * US_PER_SEC);

    // Run the event loop
    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
