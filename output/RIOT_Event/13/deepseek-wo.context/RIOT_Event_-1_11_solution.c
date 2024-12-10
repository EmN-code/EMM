
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create a periodic event
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_periodic_event_t periodic_event;
    xtimer_periodic_event_init(&periodic_event, &timer, event_periodic_callback, NULL, 1000000);

    // Start the periodic event
    xtimer_periodic_event_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_t *event = event_wait(&event_queue);
        if (event) {
            event->handler(event);
        }
    }

    return 0;
}
