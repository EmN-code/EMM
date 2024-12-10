
#include "xtimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED on ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Set up the periodic callback
    event_periodic_callback_t periodic;
    event_periodic_callback_init(&periodic, xtimer_ticks(TIMER_INTERVAL), timer_callback, NULL);

    // Start the periodic callback
    event_periodic_callback_start(&periodic);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep to avoid busy-waiting
    }

    return 0;
}
