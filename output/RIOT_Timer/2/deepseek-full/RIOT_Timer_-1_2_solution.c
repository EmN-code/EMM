
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 for the LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Wait for the timer to trigger
        xtimer_usleep(TIMER_INTERVAL);

        // Toggle the LED
        gpio_toggle(LED_PIN);
    }

    return 0;
}
