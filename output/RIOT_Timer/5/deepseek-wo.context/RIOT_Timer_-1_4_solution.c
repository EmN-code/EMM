
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to the LED

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timex_t variable to hold the interval
    timex_t interval = timex_set(1, 0);  // 1 second interval

    while (1) {
        // Toggle the LED
        gpio_toggle(LED_PIN);

        // Sleep for the specified interval
        xtimer_sleep(interval);
    }

    return 0;
}
