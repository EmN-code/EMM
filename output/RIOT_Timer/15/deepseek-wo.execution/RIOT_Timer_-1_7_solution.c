
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example LED pin for ESP32

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), (void (*)(void *))gpio_toggle, (void *)LED_PIN);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
