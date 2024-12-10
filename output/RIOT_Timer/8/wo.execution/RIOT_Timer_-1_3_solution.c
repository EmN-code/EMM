// Include necessary header files 
#include <xtimer.h>
#include <periph/gpio.h>

// Constants 
#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval set to 1 second
#define LED_PIN GPIO_PIN(PORT_X, PIN_Y)  // Define the LED pin, replace X and Y with actual port and pin numbers

// Function to toggle the LED state
void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

// Main function
int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Declare and set up a timer
    xtimer_ticks32_t last_wake_time = xtimer_now();

    while (1) {
        // Sleep for the defined interval
        xtimer_periodic_wakeup(&last_wake_time, TIMER_INTERVAL);
        // Toggle the LED state
        toggle_led();
    }
    return 0;
}