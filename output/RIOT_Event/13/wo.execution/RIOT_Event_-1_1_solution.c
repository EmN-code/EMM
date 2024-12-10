#include <xtimer.h>
#include <stdio.h>
#include <event/callback.h>

static void my_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    puts("Callback called");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_periodic_event_t event;
    
    // Set up the xtimer
    xtimer_ticks32_t tick_interval = xtimer_usec_to_ticks(1U * US_PER_SEC);

    // Initialize the periodic event
    evtimer_init_periodic(&evtimer);
    
    // Register the callback
    evtimer_periodic_add(&evtimer, &event, tick_interval, my_periodic_callback, NULL);
    
    puts("Event periodic callback initialized.");

    while (1) {
        // Let RIOT handle the events
        evtimer_periodic_run();
    }

    return 0;
}