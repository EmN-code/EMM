#include "timer.h"
#include "periph/gpio.h"

#define INTERVAL_MS    (1000U)         // 1 second interval
#define LED_PIN        GPIO_PIN(PORT_A, 0) // Define the GPIO pin for LED

tim_t timer;

// Timer callback function
static void _timer_callback(void *arg) {
    (void)arg; // Prevent unused variable warning
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with 1 second interval
    if (timer_init(&timer, 1 * 1000 * 1000, _timer_callback, NULL) < 0) {
        // Error handling (could not initialize the timer)
        return -1;
    }

    // Start the timer
    timer_start(timer);

    while (1) {
        // Keep the main function running
    }

    return 0;
}