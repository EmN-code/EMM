
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), (void *)&gpio_toggle);

    // Main loop (not strictly necessary for this example, but kept for completeness)
    while (1) {
        // The timer will handle the LED toggling
    }

    return 0;
}
