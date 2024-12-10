
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 is connected to an LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop (not strictly necessary in this case, but good practice)
    while (1) {
        // The timer will handle the LED toggling
    }

    return 0;
}
