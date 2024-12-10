#include <xtimer.h>   // Ensure this is included for xtimer related functions and types
#include <stdio.h>
#include <periph/gpio.h>

// Define the LED pin as per your ESP32 setup
#define LED_PIN GPIO_PIN(0, 2)  // Example pin, replace with actual pin used for LED

void timer_callback(void *arg) {
    (void)arg; // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle LED pin
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Setup and start the xtimer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Configure the timer to trigger every second
    uint32_t interval = 1000000; // 1 second in microseconds
    xtimer_set(&timer, xtimer_usec_to_ticks(interval));
    
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return 0;
}
