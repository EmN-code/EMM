#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

// Define the GPIO pin for the LED (adjust the number as needed for your setup)
#define LED_PIN GPIO_PIN(0, 2)  // Port 0, Pin 2

// Timer interval of 1 second
#define TIMER_INTERVAL (1U * US_PER_SEC)

static void toggle_led(void)
{
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a timer
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Wait for the next timer interval
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);

        // Call the function to toggle the LED
        toggle_led();
    }

    return 0;
}