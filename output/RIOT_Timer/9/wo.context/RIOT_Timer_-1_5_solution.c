#include <xtimer.h>  // Timer functions
#include <stdio.h>  // Standard input/output
#include <periph/gpio.h>  // GPIO functions

#define TIMER_INTERVAL (1000000U)  // Timer interval in microseconds (1 second)
#define LED_PIN GPIO_PIN(0, 2)  // Define LED pin (GPIO 2, Port 0)

void toggle_led(void) {
    // Read the current state of the LED pin
    int state = gpio_read(LED_PIN);
    // Toggle the LED state
    gpio_write(LED_PIN, state == 0 ? 1 : 0);
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize GPIO pin\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Toggle the LED
        toggle_led();

        // Sleep for the interval period
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}