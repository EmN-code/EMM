#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

static ztimer_t periodic_timer;

void callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered\n");

    /* Restart the periodic timer */
    ztimer_set(ZTIMER_SEC, &periodic_timer, 1 * ZTIMER_MSEC_PER_SEC);
}

int main(void) {
    /* Initialize the periodic timer */
    periodic_timer.callback = callback;
    periodic_timer.arg = NULL;

    /* Set the timer to trigger every second */
    ztimer_set(ZTIMER_SEC, &periodic_timer, 1 * ZTIMER_MSEC_PER_SEC);

    while (1) {
        /* Idle loop */
    }

    return 0;
}