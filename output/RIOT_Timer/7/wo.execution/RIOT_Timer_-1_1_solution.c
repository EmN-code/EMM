#include "timex.h" // Includes time utility functions
#include "periph/gpio.h" // Includes GPIO functions
#include "periph/timer.h" // Includes timer functionalities
#include <stdio.h>

// Define LED_PIN
#define LED_PIN GPIO_PIN(PORT_A, 0) // Use PORT_A pin 0, adjust according to your board

// This function toggles the LED state
void led_toggle(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a timer
    timer_t timer;
    if (timer_init(&timer, 1000000, led_toggle, NULL) < 0) { // 1 second interval
        printf("Error initializing timer\n");
        return 1;
    }

    // Start the timer
    timer_start(&timer);
    
    printf("Timer started, LED will toggle every second\n");

    // Keep the main function running
    while(1) {
        // Placeholder for other tasks
    }

    return 0;
}