#include "event/periodic.h"
#include "event/callback.h"

#include <stdio.h>

void periodic_callback(void *arg) {
    static int count = 0;
    printf("Periodic callback executed %d times\n", ++count);
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_periodic_t periodic_event;

    evtimer_set_periodic(&periodic_event, 1 * US_PER_SEC, periodic_callback, NULL);
    evtimer_init_msg(&evtimer);
    evtimer_add_periodic(&evtimer, &periodic_event);

    puts("Event timer periodic callback initialized.");

    while (1) {
        evtimer_sleep(&evtimer, 1);
    }

    return 0;
}