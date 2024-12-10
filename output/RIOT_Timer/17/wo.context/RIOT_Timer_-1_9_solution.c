#include <xtimer.h>  // for xtimer related functions
#include <stdio.h>   // for standard input/output functions
#include <periph/gpio.h> // for gpio related functions

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval defined in microseconds.
#define LED_PIN        GPIO_PIN(PORT_A, 0) // Define LED pin using the correct macro

void led_toggle(void)
{
    gpio_toggle(LED_PIN); // Toggle LED state
}

int main(void)
{
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Error initializing LED pin\n");
        return 1;
    } else {
        puts("LED pin initialized\n");
    }

    // Declare a timer variable
    xtimer_t timer;

    // Configure the timer callback
    timer.callback = led_toggle; // Set the callback function
    timer.arg = NULL;
    while (1) {
        // Set the timer
        xtimer_set(&timer, TIMER_INTERVAL);
        // Wait until the timer is triggered
        xtimer_spin_until(xtimer_now() + TIMER_INTERVAL);
    }
    return 0;
}