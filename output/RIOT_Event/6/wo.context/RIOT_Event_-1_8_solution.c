#include <ztimer.h>
#include <stdio.h>
#include <event.h>
#include <xtimer.h>

// Event type
static event_t periodic_event;

// Periodic event callback function
void callback(void *arg) {
    printf("Event callback triggered.\n");
    // Re-schedule event again after 1 second
    ztimer_set(ZTIMER_MSEC, &periodic_event.event, ZTIMER_MSEC_BASE * 1000);
}

int main(void) {
    // Initialize the event system
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Setup the periodic event
    periodic_event.event.callback = callback;
    periodic_event.event.queue = &event_queue;
    ztimer_set(ZTIMER_MSEC, &periodic_event.event, ZTIMER_MSEC_BASE * 1000);

    // Start event handling loop
    while (1) {
        event_dispatch(&event_queue);
    }
    return 0;
}