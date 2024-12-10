#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define GPIO_PIN_LED   GPIO_PIN(0, 2)

static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle the LED state
    gpio_toggle(GPIO_PIN_LED);
}

int main(void)
{
    // Initialize the LED pin
    if (gpio_init(GPIO_PIN_LED, GPIO_OUT) < 0) {
        return 1;
    }

    // Set up the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Sleep to allow timer to operate
        xtimer_usleep(TIMER_INTERVAL);
        gpio_toggle(GPIO_PIN_LED);
    }
    return 0;
}