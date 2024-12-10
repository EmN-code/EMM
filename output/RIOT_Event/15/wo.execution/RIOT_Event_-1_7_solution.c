#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>

void periodic_callback(void *arg) {
    (void)arg;  // avoid unused parameter warning
    printf("Periodic callback triggered\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    ztimer_clock_t *clock = ZTIMER_MSEC;  // Use millisecond timer

    // Initialize the periodic timer
    ztimer_periodic_init(clock, &periodic_timer, periodic_callback, NULL, 1000);
    
    // Start the timer with an interval of 1 second
    ztimer_periodic_start(&periodic_timer, 1000);

    // Let the main thread sleep to allow periodic timer callback to run
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
