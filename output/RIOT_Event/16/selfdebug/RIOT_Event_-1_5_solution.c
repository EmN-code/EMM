#include <ztimer.h>   // For timer management in RIOT
#include <stdio.h>     // Standard input/output for printf

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval set to 1 second

static void periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic callback executed");
}

int main(void) {
    ztimer_t periodic_timer;

    ztimer_init(ZTIMER_MSEC);

    // Initialize the timer to execute `periodic_callback` every second
    ztimer_set(ZTIMER_MSEC, &periodic_timer, TIMER_INTERVAL);
    periodic_timer.callback = periodic_callback;
    periodic_timer.arg = NULL;

    while (1) {
        ztimer_spin(ZTIMER_MSEC);
    }

    return 0;
}