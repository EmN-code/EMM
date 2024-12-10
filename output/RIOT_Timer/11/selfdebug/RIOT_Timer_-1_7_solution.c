#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Define the correct GPIO pin for the LED
#define TIMER_INTERVAL (1U * US_PER_SEC)  // Define the timer interval as 1 second

// Create a callback function for the timer
void timer_callback(void *arg) {
    (void)arg; // Unused
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize a timer structure
    xtimer_t timer;
    
    // Set the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}