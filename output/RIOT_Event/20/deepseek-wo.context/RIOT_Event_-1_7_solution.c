
#include "xtimer.h"
#include "event/callback.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Event periodic callback triggered\n");
}

int main(void) {
    xtimer_t timer;
    event_periodic_callback_t periodic_event;

    // Initialize the periodic event
    periodic_event.callback = event_periodic_callback;
    periodic_event.arg = NULL;
    periodic_event.interval = TIMER_INTERVAL;

    // Set the timer to trigger the periodic event
    xtimer_set_periodic(&timer, TIMER_INTERVAL, (xtimer_callback_t)event_periodic_callback, &periodic_event);

    return 0;
}
