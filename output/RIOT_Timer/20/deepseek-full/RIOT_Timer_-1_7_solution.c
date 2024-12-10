
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Adjust this to your specific LED pin

static void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create an xtimer instance
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, 1 * US_PER_SEC, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(1 * US_PER_SEC);  // Sleep for 1 second
    }

    return 0;
}
