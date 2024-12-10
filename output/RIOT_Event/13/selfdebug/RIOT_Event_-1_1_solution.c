#include <stdio.h>
#include <event/periodic.h>
#include <esp_partition.h>
#include <mutex.h>
#include <net/sock/util.h>

void periodic_callback(void *arg) {
    printf("Callback executed every second\n");
}

int main() {
    evtimer_t evtimer;
    if (evtimer_init_msg(&evtimer) == 0) {
        printf("Event timer initialized\n");
    } else {
        printf("Failed to initialize event timer\n");
        return -1;
    }

    evtimer_periodic_event_t periodic_event;
    periodic_event.event.interval = 1000000U; // Set interval to 1 second (in microseconds)
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    if (evtimer_add_periodic(&evtimer, &periodic_event, NULL) == 0) {
        printf("Periodic event added\n");
    } else {
        printf("Failed to add periodic event\n");
        return -1;
    }

    while (1) {
        // Main loop can contain other code or be empty if only the callback is needed
        // evtimer_run is called by the RIOT scheduler, so no need to explicitly call it here
    }

    return 0;
}