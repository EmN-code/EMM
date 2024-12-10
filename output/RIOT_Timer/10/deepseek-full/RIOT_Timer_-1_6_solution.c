
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 5)  // Example GPIO pin for LED on ESP32
#define TIMER_INTERVAL 1000000  // 1 second in microseconds

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the LED pin
    xtimer_init();  // Initialize the xtimer

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Main loop, nothing to do here as the timer handles everything
    }

    return 0;
}
