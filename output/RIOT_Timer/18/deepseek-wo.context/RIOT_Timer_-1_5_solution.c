
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Toggle the LED
        gpio_toggle(LED_PIN);

        // Set the timer to trigger every second
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}
