#include "event/periodic.h"

#define INTERVAL (1U * US_PER_SEC)

void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_periodic_t periodic_timer;

    evtimer_periodic_init(&periodic_timer, INTERVAL, callback, NULL);
    evtimer_periodic_start(&periodic_timer);

    while (1) {
        evtimer_periodic_dispatch();
    }

    return 0;
}