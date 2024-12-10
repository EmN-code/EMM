
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is the LED pin on ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000UL)  // 1 second in microseconds

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_periodic_wakeup(&timer.target, TIMER_INTERVAL);

    while (1) {
        // Toggle the LED
        gpio_toggle(LED_PIN);

        // Wait for the next timer tick
        xtimer_periodic_wakeup(&timer.target, TIMER_INTERVAL);
    }

    return 0;
}
