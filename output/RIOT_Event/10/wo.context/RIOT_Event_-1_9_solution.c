#include "ztimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void _callback(void *arg) {
    (void)arg;
    puts("Periodic callback executed");
}

int main(void) {
    puts("Starting periodic event example");

    evtimer_t timer;
    evtimer_event_t event;

    /* Initialize periodic timer */
    evtimer_init(&timer, _callback);
    evtimer_add(&timer, &event, TIMER_INTERVAL);

    /* Main loop */
    while (1) {
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}