// Including necessary header files
#include "timex.h"
#include "thread.h"
#include "timex.h"
#include "periph/gpio.h"
#include "xtimer.h"

// Define the LED pin (GPIO) according to your setup
#define LED_PIN GPIO_PIN(0, 2) // Example pin, please change according to your actual setup

// This is the callback function which will be called every time the timer expires
void timer_callback(void *arg) {
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a timer that triggers every second
    static const uint32_t TIMER_INTERVAL = 1 * US_PER_SEC;  // 1 second
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Invoke the callback function
        timer_callback(NULL);
        
        // Wait for the next timer interrupt
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}