#include <xtimer.h>  // For using the timer functions
#include <stdio.h>
#include <periph/gpio.h>  // For GPIO operations

#define LED_PIN GPIO_PIN(0, 2)  // Assuming LED is connected to GPIO2

void toggle_led(void) {
    static int led_state = 0;
    led_state = !led_state;
    gpio_write(LED_PIN, led_state);
}

int main(void) {
    printf("Starting LED toggle timer example...\n");

    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize GPIO pin\n");
        return 1;
    }

    // Set initial LED state to off
    gpio_write(LED_PIN, 0);

    // Run the timer loop
    while (1) {
        toggle_led();
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}