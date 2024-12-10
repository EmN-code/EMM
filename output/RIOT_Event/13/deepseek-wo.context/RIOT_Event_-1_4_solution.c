
#include "xtimer.h"
#include "event/callback.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the periodic timer
    xtimer_init(&timer);

    // Set the callback function for the periodic timer
    timer.callback = periodic_callback;

    // Start the periodic timer
    xtimer_periodic_wakeup(&timer, PERIODIC_INTERVAL);

    // Keep the main thread running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
