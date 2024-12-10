#include "xtimer.h"
#include "periph/gpio.h"

// Function to toggle LED state
void toggle_led(void *arg) {
    (void)arg; // Unused parameter
    gpio_toggle(GPIO_PIN(0, 2));
}

int main(void) {
    // Initialize the LED pin
    gpio_init(GPIO_PIN(0, 2), GPIO_OUT);
    
    // Set up a 1-second timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
        toggle_led(NULL);
    }
    return 0;
}