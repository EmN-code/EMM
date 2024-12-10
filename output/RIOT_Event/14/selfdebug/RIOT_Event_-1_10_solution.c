#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>

#define PERIODIC_INTERVAL_MS 1000  // 1 second in milliseconds

static void event_periodic_callback(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    ztimer_t periodic_timer;
    periodic_timer.callback = event_periodic_callback;
    periodic_timer.arg = NULL;

    // Initialize the ztimer
    if (ztimer_periodic_start(&ztimer_msec, &periodic_timer, PERIODIC_INTERVAL_MS, PERIODIC_INTERVAL_MS) != 0) {
        printf("Failed to start periodic timer\n");
        return EXIT_FAILURE;
    }

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);  // Sleep for 1 second
    }

    return EXIT_SUCCESS;
}