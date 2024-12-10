#include "event/periodic.h"

#include <stdio.h>

void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}
int main() {
    evtimer_periodic_t periodic_event;
    evtimer_init_periodic(&periodic_event);
    periodic_event.callback = callback;
    periodic_event.arg = NULL;
    evtimer_periodic_add(&periodic_event, 1000000);
    printf("Event periodic initialized and started\n");
    while (1) {
        /* Simulate idle loop to keep main thread running */
    }
    return 0;
}
