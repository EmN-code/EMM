
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer with the callback function and interval
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every INTERVAL_MS milliseconds
    xtimer_set_periodic(&timer, INTERVAL_MS);

    // Main loop to keep the application running
    while (1) {
        // Add any additional logic here if needed
    }

    return 0;
}
