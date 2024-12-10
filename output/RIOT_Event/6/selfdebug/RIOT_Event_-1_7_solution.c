#include <xtimer.h>
#include <event/callback.h>
#include <event.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

evimer_t evtimer;
event_callback_t evtimer_callback;

void my_event_handler(void *arg) {
    (void)arg; // Unused argument
    puts("Event triggered every second!");
}

int main(void) {
    evtimer_callback.callback = my_event_handler;
    xtimer_t timer;
    evtimer_add(&evtimer_callback);

    printf("Setting up periodic timer with %u microseconds interval.\n", TIMER_INTERVAL);
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        evtimer_dispatch(evcq_get(EVENT_QUEUE)
    }

    return 0;
}