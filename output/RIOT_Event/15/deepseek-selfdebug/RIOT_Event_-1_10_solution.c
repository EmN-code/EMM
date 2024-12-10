
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    // Create a periodic event
    periodic_event_t periodic_event;
    periodic_event_init(&periodic_event, &event_queue, periodic_event_callback, NULL);

    // Set the periodic event to trigger every second
    xtimer_periodic_event_init_msg(&periodic_event, 1 * US_PER_SEC);

    // Start the periodic event
    periodic_event_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
