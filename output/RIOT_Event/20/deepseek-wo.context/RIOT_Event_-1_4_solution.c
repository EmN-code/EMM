
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the periodic callback function
    xtimer_set_periodic(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Main loop
    while (1) {
        // Your main loop code here
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
