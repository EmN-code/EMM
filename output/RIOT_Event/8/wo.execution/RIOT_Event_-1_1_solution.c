#include <stdio.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Callback function called every second\n");
}

int main() {
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_periodic_init(&evtimer);
    evtimer_set_absolute(&evtimer, &event, 1000, &callback);
    evtimer_start(&evtimer);

    while (1) {
        evtimer_process(&evtimer);
    }
    return 0;
}