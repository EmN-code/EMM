
#include "periph/gpio.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED

void toggle_led(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), toggle_led, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
