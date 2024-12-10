
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 is connected to an LED

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_set(&timer, XTIMER_TICKS(1000000), timer_callback, NULL);

    // Main loop (not strictly necessary in this case, but good practice)
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
