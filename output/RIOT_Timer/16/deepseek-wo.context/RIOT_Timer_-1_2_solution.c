
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED

int main(void)
{
    xtimer_t timer;
    gpio_init(LED_PIN, GPIO_OUT);

    xtimer_init(&timer);
    xtimer_set(&timer, 1 * US_PER_SEC);  // Set timer to trigger every second

    while (1) {
        gpio_toggle(LED_PIN);  // Toggle the LED state
        xtimer_usleep(1 * US_PER_SEC);  // Sleep for 1 second
    }

    return 0;
}
