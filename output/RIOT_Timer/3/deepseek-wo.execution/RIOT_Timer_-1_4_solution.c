
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds
#define LED_PIN GPIO_PIN(0, 16) // Example LED pin for ESP32

static void _timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, _timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // The timer callback will handle the LED toggling
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
