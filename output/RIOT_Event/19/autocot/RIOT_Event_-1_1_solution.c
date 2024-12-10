#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

void my_event_callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    uint32_t interval_usec = 1000000; // 1 second interval in microseconds

    // Initialize event periodic
    event_periodic_init(&periodic_event);

    // Set callback for the periodic event
    event_callback_init(&periodic_event.callback, my_event_callback, NULL);

    // Start the periodic event
    event_periodic_set(&periodic_event, interval_usec);

    // Start the event loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}