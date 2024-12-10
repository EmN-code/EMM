#include <xtimer.h>   // Require for xtimer_set
#include <stdio.h>     // Standard input-output
#include <periph/gpio.h> // Require for gpio initialization and control

#define LED_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for LED, replace with the desired pin number.
#define TIMER_INTERVAL (1U * US_PER_SEC) // Define the interval for the timer in microseconds

void toggle_led(void) {
    static int led_status = 0;
    
    if (led_status) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_status = !led_status;
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED_PIN\n");
        return 1;
    }

    // Timer structure for periodic callbacks
    xtimer_t timer;

    // Callback function
    timer.callback = (void (*)(void *))toggle_led;
    timer.arg = NULL;

    // Set the timer to trigger every TIMER_INTERVAL microseconds
    while(1) {
        xtimer_set(&timer, TIMER_INTERVAL);
    }

    return 0;
}