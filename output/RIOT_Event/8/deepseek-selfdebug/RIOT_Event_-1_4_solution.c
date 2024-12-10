
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event.handler = periodic_event_callback;
    periodic_event.next = NULL;

    // Start the periodic event
    while (1) {
        xtimer_sleep(1);
        event_post(&event_queue, &periodic_event);
    }

    return 0;
}
