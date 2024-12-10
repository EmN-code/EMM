#include <xtimer.h>  // Necessary for xtimer functionality
#include <stdio.h>  // Standard C library for input output functions
#include "timex.h"  // Needed for time conversion
#include "periph/gpio.h"  // Provides access to GPIO pin functionality

// GPIO pin to toggle LED
#define LED_PIN GPIO_PIN(0, 2)

// Function to initialize the timer and GPIO
void initialize(void) {
    // Initialize GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error: unable to initialize LED\n");
        return;
    }

    printf("GPIO initialized, starting timer\n");
}

// Callback function to be triggered by the timer
void timer_callback(void* arg) {
    (void)arg;
    // Toggle LED
    gpio_toggle(LED_PIN);
}


int main(void) {
    // Initialize GPIO
    initialize();

    // Define and set the timer to 1-second intervals
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, 1000000UL);

    // Enter an infinite loop to keep the program running
    while (1) {
        // Sleep for a while to reduce CPU load
        xtimer_sleep(1);
    }

    return 0;
}