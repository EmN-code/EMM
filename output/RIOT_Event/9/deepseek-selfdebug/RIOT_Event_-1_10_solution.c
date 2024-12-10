
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, INTERVAL_MS * 1000);  // Convert milliseconds to microseconds

    while (1) {
        // Main loop
    }

    return 0;
}
