
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create a periodic event
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, 1 * US_PER_SEC, (xtimer_callback_t)periodic_callback, NULL);

    // Run the event loop
    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
