
#include "periph/gpio.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(0, 13)  // Example GPIO pin for LED

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a periodic timer to trigger every second
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, XTIMER_TICKS(1 * US_PER_SEC), (void (*)(void *))toggle_led, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1 * US_PER_SEC);
    }

    return 0;
}
