
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to an LED

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    gpio_toggle(LED_PIN);
    xtimer_set(&timer, xtimer_ticks(1 * US_PER_SEC));  // Set timer to trigger every second
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the LED pin as output

    // Initialize the timer and set the callback function
    xtimer_init(&timer);
    xtimer_set(&timer, xtimer_ticks(1 * US_PER_SEC));  // Set timer to trigger every second

    while (1) {
        // Main loop
    }

    return 0;
}
