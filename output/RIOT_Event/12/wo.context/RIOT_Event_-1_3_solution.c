#include "ztimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DEBUG_PIN GPIO_PIN(PORT_A, 5)  // Define the pin for debugging
#define INTERVAL_USEC (1U * 1000000U)  // 1 second in microseconds

void callback_function(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
    gpio_toggle(DEBUG_PIN);  // Toggle pin for debugging
}

void main(void) {
    // Initialize GPIO pin for debugging
    gpio_init(DEBUG_PIN, GPIO_OUT);

    // Create a ztimer instance
    ztimer64_t timer;

    // Set the timer to call callback_function every second
    ztimer64_set_periodic(ZTIMER_MSEC, &timer, INTERVAL_USEC, callback_function, NULL);

    printf("Event periodic callback started!\n");

    // If you want to spin until a certain time with the new API:
    ztimer64_ticker_t *t = ZTIMER64_TICKS_TO_SET(ZTIMER_MSEC);
    while (1) {
        ztimer64_spin_until(t, INTERVAL_USEC);
    }
}