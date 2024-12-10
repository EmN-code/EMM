#include <xtimer.h>    // For timer functions
#include <stdio.h>    // For printf function
#include <periph/gpio.h>    // For GPIO functions

#define LED_PIN GPIO_PIN(PORT_A, 5)    // Define the GPIO pin connected to the LED, adjust according to your setup

void timer_callback(void *arg) {
    (void)arg;    // Suppress unused parameter warning
    gpio_toggle(LED_PIN);    // Toggle the LED state
    printf("LED Toggled!\n");
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);    // Initialize the LED_PIN as output
    xtimer_ticks32_t interval = xtimer_usec_from_sec(1);    // Set timer to 1 second interval

    while (1) {
        xtimer_ticks32_t last_wakeup = xtimer_now();
        timer_callback(NULL);    // Execute the timer callback
        xtimer_periodic_wakeup(&last_wakeup, interval);    // Sleep for the interval duration
    }
    return 0;
}