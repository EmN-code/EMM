#include <event/periodic.h>
#include <event/callback.h>

void callback(void *arg) {
    // This function will be called every second
    printf("Periodic callback executed.\n");
}

int main(void) {
    // Initialize event structure
    evtimer_periodic_t periodic_evtimer;
    evtimer_periodic_init(&periodic_evtimer);

    // Setting up a callback event for a timer periodic event
    callback_t my_callback;
    my_callback.event.handler = callback;
    my_callback.event.arg = NULL;

    // Initialize a periodic event to call the `callback` function every second (1000 ms)
    evtimer_periodic_add(&periodic_evtimer, &my_callback.event, 1000);

    // Enter the main loop
    while (true) {
        evtimer_periodic_dispatch(&periodic_evtimer);
    }

    return 0;
}