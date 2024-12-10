
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize the timer with the callback function
    xtimer_init(&timer);
    xtimer_set(&timer, period, timer_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // The timer will handle the LED toggling
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
