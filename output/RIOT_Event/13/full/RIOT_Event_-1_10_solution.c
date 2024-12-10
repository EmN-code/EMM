#include "evtimer.h"
#include "board.h"
#include "kernel.h"

void timer_callback(void *arg) {
    (void)arg;
    puts("Callback function triggered.");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer, timer_callback);

    while (1) {
        evtimer_set(&evtimer, &event, 1000 * US_PER_MS); // Set timer for 1 second (1000 milliseconds)
        evtimer_handler(&evtimer, &event);
    }

    return 0;
}