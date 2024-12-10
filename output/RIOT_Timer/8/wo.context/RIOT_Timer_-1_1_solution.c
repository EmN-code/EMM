#include <xtimer.h>  // Include the xtimer for timer functions
#include <stdio.h>    // Standard input/output library
#include <periph/gpio.h>  // Include the GPIO peripheral library for LED

#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the GPIO pin for the LED

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle the LED state
    puts("LED toggled");  // Print message
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the GPIO pin as output
    
    // Create a timer that triggers the callback every 1 second
    xtimer_ticks32_t interval = xtimer_sec(1);
    xtimer_set_cb(NULL, &interval, timer_callback);

    while (1) {
        // Main loop does nothing; timer runs independently
        xtimer_sleep(1);  // Sleep for 1 second to reduce CPU usage
    }

    return 0;  // Never reached
}