
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 is connected to an LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static xtimer_t timer;

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Wait for the timer to expire
        xtimer_usleep(TIMER_INTERVAL);

        // Toggle the LED
        led_toggle();
    }

    return 0;
}
