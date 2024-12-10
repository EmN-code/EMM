#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"

#define INTERVAL (1U * 1000000U) // Timer interval set to 1 second (in microseconds)
#define LED_PIN GPIO_PIN(PORT_A, 0) // Specify the correct PORT and PIN number here

static void led_toggle(void)
{
    static int state = 0;
    if (state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    state = !state;
}

int main(void)
{
    // Initialize the GPIO as output
    if(gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;
    }

    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        led_toggle();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }
    return 0;
}
