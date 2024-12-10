
#include "xtimer.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer with the callback function and argument
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger every INTERVAL_MS milliseconds
    xtimer_set(&timer, INTERVAL_MS);

    // Main loop to keep the application running
    while (1) {
        // The periodic callback will be triggered by the xtimer
    }

    return 0;
}
