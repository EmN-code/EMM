
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    // Create a periodic event
    xtimer_t timer;
    xtimer_init(&timer, XTIMER_DEV, XTIMER_CHAN);
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), (xtimer_callback_t)event_periodic_callback, NULL);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
