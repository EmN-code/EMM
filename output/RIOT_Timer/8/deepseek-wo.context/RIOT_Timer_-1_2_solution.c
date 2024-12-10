
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to an LED

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_init(&timer, XTIMER_TICKS(1 * US_PER_SEC));

    while (1) {
        // Turn on the LED
        gpio_set(LED_PIN);

        // Wait for 1 second
        xtimer_sleep(&timer, 1);

        // Turn off the LED
        gpio_clear(LED_PIN);

        // Wait for 1 second
        xtimer_sleep(&timer, 1);
    }

    return 0;
}
