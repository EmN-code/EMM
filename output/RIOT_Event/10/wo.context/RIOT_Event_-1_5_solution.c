#include <xtimer.h>
#include <evtimer.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

event_t periodic_event;
evtimer_t evtimer;

void periodic_callback(event_t *event)
{
    // Code to execute every period
    puts("Periodic callback triggered.");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer, NULL);

    // Set up a periodic event
    evtimer_event_init(&periodic_event, periodic_callback);
    evtimer_add(&evtimer, &periodic_event, TIMER_INTERVAL);

    puts("Timer initialized, will trigger periodic events every second.");

    while (1) {
        // Sleep for defined interval to let the periodic event trigger
        evtimer_dispatch(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}