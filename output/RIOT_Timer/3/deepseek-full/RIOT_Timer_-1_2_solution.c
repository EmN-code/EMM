
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PA, 0)  // Adjust this to your specific LED pin

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Create a periodic timer that triggers every second
    xtimer_t timer;
    xtimer_periodic_callback_t periodic_callback;
    xtimer_periodic_callback_create(&periodic_callback, &timer, 1 * US_PER_SEC, &timer_callback, NULL);

    // Start the periodic timer
    xtimer_periodic_callback_start(&periodic_callback);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1 * US_PER_SEC);
    }

    return 0;
}
