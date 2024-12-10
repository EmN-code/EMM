#include <stdio.h>
#include <evtimer.h>

evtimer_t evtimer;
evtimer_event_t event;

void event_periodic(void *arg) {
    printf("Periodic event triggered\n");
    // Set the event again for 1 second later
    xtimer_set(&((evtimer_t*)arg)->timer, 1000000); 
}

int main(void) {
    // Initialize the evtimer
    evtimer_init(&evtimer, (evtimer_callback_t)event_periodic);

    // Set the event to trigger every second (1,000,000 microseconds)
    evtimer_set(&evtimer, &event, event_periodic);

    // This would normally go in a main loop, simplified here
    while (1) {
        evtimer_handle(&evtimer);
        sleep(1);
    }

    return 0;
}