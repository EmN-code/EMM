
#include "xtimer.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    // Your periodic event code here
    printf("Periodic event triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every second
    xtimer_periodic_wakeup(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Your main loop code here
    }

    return 0;
}
