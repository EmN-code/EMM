#include <xtimer.h> // Corrected include for timer functions
#include "periph/gpio.h"
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(0, 2)

static void led_toggle(void)
{
    gpio_toggle(LED_PIN);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Continuously toggle LED every second
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        led_toggle();
    }

    return 0;
}