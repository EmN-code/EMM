
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), PRIORITY_MAIN);

    // Create a periodic event
    xtimer_t periodic_timer;
    periodic_timer.callback = event_periodic_callback;
    periodic_timer.arg = NULL;

    // Start the periodic event with a 1-second interval
    xtimer_set(&periodic_timer, XTIMER_TICKS(1000000));

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
