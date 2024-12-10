#include <xtimer.h> // Include for timer functionality
#include <string.h> // Required for string operations
#include <stdio.h>  // Standard input/output definitions
#include <stdlib.h> // Standard library functions
#include "periph/gpio.h"  // Include for GPIO operations

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval of 1 second
#define LED_PIN        GPIO_PIN(0, 0)    // Define LED pin in a valid format

void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;

    led_state = !led_state; // Toggle LED state
    gpio_write(LED_PIN, led_state ? 1 : 0); // Set LED state
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set a periodic timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Enter the main loop (typically, a RIOT application would not return from main)
    while(1) {
        
        xtimer_sleep(10); 
    }

    return 0; 
}