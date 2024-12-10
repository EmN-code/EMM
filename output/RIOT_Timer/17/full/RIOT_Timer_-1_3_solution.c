#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>

// The pin where the LED is connected
#define LED_PIN GPIO_PIN(PORT_A, 2)

// Time interval for the XTIMER, here 1 second (1,000,000 microseconds)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Function to toggle LED
void led_toggle(void)
{
    static bool led_state = false;
    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

int main(void)
{
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize LED_PIN\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Set up a recurring timer
    while (1) {
        led_toggle();
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}