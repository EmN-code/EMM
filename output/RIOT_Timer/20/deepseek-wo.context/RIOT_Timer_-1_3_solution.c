
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Adjust this pin according to your ESP32 setup
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
