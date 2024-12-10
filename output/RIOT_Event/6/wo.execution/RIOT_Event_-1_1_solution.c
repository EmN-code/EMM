#include <xtimer.h> // Timer facility for RIOT
#include <stdio.h> // Standard input/output
#include <event/periodic.h> // Event periodic callback

#define INTERVAL (1U * US_PER_SEC) // Interval of one second in microseconds

static void callback(void *arg) {
    (void)arg; // Silence unused parameter warning
    printf("Callback function called every second\n");
}

int main(void) {
    // Initialize and configure the periodic event
    evtimer_periodic_t periodic_event;
    evtimer_event_t event; 

    // Setup the periodic event
    evtimer_init(&periodic_event.timer, &event);
    event.callback = callback;
    event.arg = NULL;
    evtimer_add_periodic(&periodic_event, INT32_MAX, INTERVAL);

    // Enter an infinite loop to keep the main function running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}