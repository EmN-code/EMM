#include <xtimer.h> // Handles timers in RIOT
#include <stdio.h>
#include <periph/gpio.h> // Controls GPIO pins

#define LED_PIN GPIO_PIN(0, 2) // Define the LED pin

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT); // Initialize the GPIO pin for the LED
    xtimer_ticks32_t interval = xtimer_usec_to_ticks(1000000); // Set interval to 1 second
    while (1) {
        xtimer_periodic_wakeup(NULL, interval); // Wait for the interval
        timer_callback(NULL); // Execute the callback to toggle LED
    }
    return 0;
}