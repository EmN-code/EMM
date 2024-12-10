#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static ztimer_periodic_t periodic_timer;

void callback(void *arg) {
    (void)arg;
    printf("Periodic timer callback triggered!\n");
}

int main(void) {
    /* initialize the periodic timer */
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, callback, NULL, TIMER_INTERVAL);

    /* start the periodic timer */
    ztimer_periodic_start(&periodic_timer);

    /* Main loop */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}