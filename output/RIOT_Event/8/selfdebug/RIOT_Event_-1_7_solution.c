#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

static ztimer_periodic_t my_timer;

void periodic_callback(void *arg) {
    (void)arg;  // Suppress unused argument warning
    printf("Periodic callback executed!\n");
}

int main(void) {
    // Initialize timer for a 1 second tick
    ztimer_periodic_init(ZTIMER_MSEC, &my_timer, periodic_callback, NULL, 1000);

    // Start periodic execution
    while(1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, &my_timer);
    }

    return 0;
}