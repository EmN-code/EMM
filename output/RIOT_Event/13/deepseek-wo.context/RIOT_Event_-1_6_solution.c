
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define INTERVAL_MS (1000U)  // 1 second interval

void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    uint32_t last_wakeup = xtimer_now().ticks32;

    // Initialize the timer and set the periodic callback
    xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    xtimer_set(&timer, INTERVAL_MS, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        xtimer_set(&timer, INTERVAL_MS, periodic_callback, NULL);
    }

    return 0;
}
