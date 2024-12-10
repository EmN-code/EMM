#include <stdio.h>  // For printf function
#include "periph/timer.h"  // For timer functions
#include "periph/gpio.h"  // For controlling GPIOs

// Define LED GPIO pin (assuming GPIO 2 as an example)
#define LED_PIN GPIO_PIN(0, 2)

// Callback function to toggle the LED
void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    gpio_toggle(LED_PIN);
    printf("LED toggled.\n");
}

int main(void) {
    // Initialize GPIO as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Define timer parameters
    tim_t timer = TIMER_0;
    const uint32_t frequency = 1; // 1 Hz frequency for a 1-second timer
    
    // Initialize the timer
    int res = timer_init(timer, frequency, timer_callback, NULL);
    if (res != 0) {
        printf("Timer initialization failed with code %d.\n", res);
        return 1;
    }
    
    // Start the timer
    timer_start(timer);
    printf("Timer started, toggling LED every second.\n");
    
    while (1) {
        // Main loop does nothing, timer callback handles LED toggling
    }
    return 0;
}