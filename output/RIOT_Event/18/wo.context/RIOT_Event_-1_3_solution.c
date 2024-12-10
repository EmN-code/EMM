#include <ztimer.h>
#include <stdio.h>

#define PERIOD (1U * ZTIMER_MSEC_PER_SEC)

void event_periodic_callback(void *arg) {
    puts("Event periodic callback triggered.");
}

int main(void) {
    ztimer_t timer;
    ztimer_callback_init(&timer, event_periodic_callback, NULL);
    ztimer_set(&timer, PERIOD);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, PERIOD);
    }

    return 0;
}