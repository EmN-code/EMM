#include <xtimer.h>      // For ztimer usage
#include <stdlib.h>      // For standard library functions
#include <periph/gpio.h> // For GPIO operations

// Define the GPIO pin for the LED
#define LED_PIN GPIO_PIN(0, 2) // Example LED pin, change as needed

// Timer interval in microseconds (1 second)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

// Main function
int main(void) {
    // Initialize the LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer and configure it to trigger every second
    ztimer_t timer;
    ztimer_callback_init(ZTIMER_USEC, &timer, timer_callback, NULL);
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Start a periodic timer that triggers every TIMER_INTERVAL microseconds
    ztimer_periodic_wakeup_usec(1, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Sleep until the next timer period
        ztimer_periodic_wakeup_usec(1, TIMER_INTERVAL);
    }

    return 0;
}