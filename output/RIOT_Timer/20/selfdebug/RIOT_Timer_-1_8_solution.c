#include "ztimer.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define INTERVAL_SEC (1)  // Periodic interval in seconds
#define LED_PIN GPIO_PIN(0, 2)

// Function to toggle LED state each time the timer triggers
void timer_callback(void *arg) {
    (void)arg; // Suppress unused parameter warning
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a ztimer to trigger every second
    ztimer_t timer;
    ztimer_set_handler(&timer, ZTIMER_SEC, timer_callback, NULL);
    ztimer_set(&timer, ztimer_usec_from_sec(INTERVAL_SEC), ztimer_callback);

    // Allow the program to continue running to service the timer
    while (1) {
        ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);
    }

    return 0;
}