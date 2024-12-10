
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 on ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Set up the periodic timer
    xtimer_periodic_wakeup(NULL, TIMER_INTERVAL);

    // Set the timer callback
    xtimer_set_cb(timer_callback, NULL);

    // Main loop
    while (1) {
        // The timer callback will be called periodically
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
