
#include "xtimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void _timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a periodic callback
    periodic_callback_t timer;
    periodic_callback_init(&timer, xtimer_ticks32(TIMER_INTERVAL), _timer_callback, NULL);

    // Start the periodic callback
    periodic_callback_start(&timer);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
