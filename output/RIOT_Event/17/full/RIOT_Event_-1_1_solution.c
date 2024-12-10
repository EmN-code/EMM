#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

#define INTERVAL_S 1  // 1 second

// Event handler
void _my_periodic_cb(void *arg) {
    (void)arg;
    puts("Event triggered every second.");
}

int main(void) {
    puts("RIOT OS Event Timer Example");

    // Define a periodic event
    event_t periodic_event;
    evtimer_periodic(&periodic_event, INTERVAL_S * US_PER_SEC, _my_periodic_cb, NULL);

    // Set up as a callback for periodic execution
    event_callback_t callback_event;
    event_callback_init(&callback_event, &periodic_event);

    // Start the event loop
    while (1) {
        thread_yield();
    }

    return 0;
}