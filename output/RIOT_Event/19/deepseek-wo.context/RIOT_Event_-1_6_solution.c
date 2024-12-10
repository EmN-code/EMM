
#include "xtimer.h"
#include "event.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    char stack[THREAD_STACKSIZE_DEFAULT];

    // Initialize the event queue
    event_thread_init(&event_queue, stack, sizeof(stack), THREAD_PRIORITY_MAIN);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, event_periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event, TIMER_INTERVAL);

    // Run the event loop
    event_loop(&event_queue);

    return 0;
}
