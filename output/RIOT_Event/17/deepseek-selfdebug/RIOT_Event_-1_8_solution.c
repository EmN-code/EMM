
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, event_thread_stack, EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIORITY);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, event_periodic_callback, NULL);

    // Set the periodic event to trigger every second
    xtimer_periodic_event_init(&periodic_event, xtimer_ticks_from_usec(1000000));

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
