
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is the LED pin on ESP32

int main(void)
{
    // Initialize the xtimer module
    xtimer_init();

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Toggle the LED
        gpio_toggle(LED_PIN);

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
