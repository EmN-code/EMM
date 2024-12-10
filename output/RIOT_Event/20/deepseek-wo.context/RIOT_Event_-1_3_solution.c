
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    xtimer_init();  // Initialize the xtimer

    // Set up the periodic callback
    xtimer_set_periodic(&timer, INTERVAL, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
