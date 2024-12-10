
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/periodic.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t queue;
static event_periodic_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_ticks32(xtimer_s2ticks(1)), periodic_callback, NULL);

    // Add the periodic event to the queue
    event_periodic_add(&queue, &periodic_event);

    // Start the event thread
    event_thread_init(&queue, stack, STACKSIZE, PRIORITY);

    return 0;
}
