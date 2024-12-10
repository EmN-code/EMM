
#include "xtimer.h"
#include "event.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_queue_t event_queue;
static char stack[THREAD_STACKSIZE_DEFAULT];

void event_periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Event periodic callback triggered\n");
}

int main(void) {
    // Initialize the event queue
    event_thread_init(&event_queue, stack, sizeof(stack), THREAD_PRIORITY_MAIN);

    // Create a periodic event
    event_periodic_t periodic_event;
    periodic_event.event.handler = event_periodic_callback;
    periodic_event.event.arg = NULL;
    periodic_event.interval = TIMER_INTERVAL;

    // Start the periodic event
    event_periodic_create(&periodic_event, &event_queue);

    // Main loop to keep the thread alive
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
