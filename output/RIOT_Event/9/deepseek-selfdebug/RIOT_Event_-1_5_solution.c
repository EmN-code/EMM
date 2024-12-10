
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
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
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    // Create a periodic event
    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    // Start the periodic event every second
    xtimer_set(&timer, 1000000);

    // Process events
    event_loop(&event_queue);

    return 0;
}
