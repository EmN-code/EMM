#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

void periodic_callback(void *arg) {
    printf("Callback called every second\n");
}

int main(void) {
    printf("Starting event_periodic example.\n");

    evtimer_t periodic_timer;
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    evtimer_periodic_init(&periodic_timer, EV_PERIODIC_RESTARTING, interval, periodic_callback, NULL);
    evtimer_periodic_start(&periodic_timer);

    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}