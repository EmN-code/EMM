
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Define the periodic event callback structure
    event_periodic_callback_t periodic_event;

    // Create and start the periodic callback event
    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, 1000000, &event_queue, periodic_callback, NULL);

    // Start the periodic callback
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
