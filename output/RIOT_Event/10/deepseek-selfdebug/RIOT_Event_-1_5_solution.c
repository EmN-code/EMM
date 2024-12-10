
#include <stdio.h>
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
    printf("Event periodic callback triggered!\n");
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

    // Set the timer to trigger every second
    xtimer_set(&timer, 1000000);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
