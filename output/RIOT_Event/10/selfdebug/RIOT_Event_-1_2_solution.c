#include <stdio.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>

static ztimer_t timer;

void event_periodic_callback(void *arg) {
    (void)arg; // avoid unused parameter warning
    printf("Periodic event triggered!\n");
}

void event_periodic_init(ztimer_t *timer, ztimer_callback_t callback, void *arg) {
    timer->callback = callback;
    timer->arg = arg;
    timer->longtimer = 0;
}

void event_periodic_start(ztimer_t *timer, uint32_t interval) {
    xtimer_set(&timer->longtimer, interval);
}

int main(void) {
    event_periodic_init(&timer, event_periodic_callback, NULL);
    event_periodic_start(&timer, 1 * US_PER_SEC);
    
    while (1) {
        xtimer_usleep(1 * US_PER_SEC); // sleep for a second before loop continues
    }
    return 0;
}