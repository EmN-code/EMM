
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

void event_periodic_callback(void *arg)
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
    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)event_periodic_callback;
    periodic_timer.arg = NULL;

    // Set the periodic timer to trigger every second
    xtimer_set(&periodic_timer, 1000000);

    // Start the event loop
    event_loop(&event_queue);

    return 0;
}