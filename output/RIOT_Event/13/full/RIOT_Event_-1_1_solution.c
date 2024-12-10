#include "event/periodic.h"
#include "event/callback.h"
#include "esp_partition.h"

void callback(void *arg) {
    printf("Callback function executed every second.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init(&evtimer, _evtimer_msg_handler);

    evtimer_periodic_event_t periodic_event;
    periodic_event.callback = callback;
    periodic_event.period = 1000000; // Set period to 1 second (1,000,000 microseconds)
    periodic_event.arg = NULL;

    evtimer_add_periodic(&evtimer, &periodic_event);

    // Start the event loop
    while (1) {
        evtimer_periodic_evtimer_callback(&evtimer);
    }
    return 0;
}