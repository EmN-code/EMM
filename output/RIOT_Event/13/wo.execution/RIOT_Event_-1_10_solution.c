#include <xtimer.h>
#include <event.h>
#include <msg.h>
#include <stdio.h>
#include <evtimer.h>

static void my_callback(void *arg) {
    (void)arg;
    puts("Event triggered!");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_set(&event, my_callback, NULL);
    evtimer_init(&evtimer, NULL);
    evtimer_add(&evtimer, &event, xtimer_usec_to_ticks(1000000));

    while (1) {
        evtimer_sleep(&evtimer);
    }

    return 0;
}
