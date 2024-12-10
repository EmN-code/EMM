#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Example for using GPIO pin

void timer_callback(void)
{
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    // Set up the timer
    xtimer_ticks32_t interval = xtimer_ticks_from_seconds(1);
    while (1) {
        // Wait for the interval
        xtimer_periodic_wakeup(NULL, interval);
        // Call the timer callback function
        timer_callback();
    }

    return 0;
}