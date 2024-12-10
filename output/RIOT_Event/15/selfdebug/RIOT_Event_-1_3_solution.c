#include <ztimer.h>
#include <stdio.h>
#include <event/callback.h>

#define TIMER_INTERVAL (1U * ZTIMER_MSEC_PER_SEC)

ztimer_periodic_t periodic_timer;

void timer_callback(void *arg) {
    (void)arg; // Unused
    printf("Timer fired!\n");
}

int main(void) {
    // Initialize the periodic timer
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, timer_callback, NULL, TIMER_INTERVAL);
    // Start the periodic timer
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        // Enter low-power mode while waiting for the next interrupt
        ztimer_periodic_sleep_until(&periodic_timer, ztimer_now(ZTIMER_MSEC) + TIMER_INTERVAL);
    }

    return 0;
}