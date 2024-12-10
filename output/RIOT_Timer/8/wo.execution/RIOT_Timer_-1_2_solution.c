#include <xtimer.h>  // Include necessary header for xtimer
#include <stdio.h>    // Include standard I/O for logging
#include <periph/gpio.h> // Include for GPIO peripheral support

// Define the LED pin (this might change depending on your ESP32 board configuration)
#define LED_PIN GPIO_PIN(0, 2)  // Example for a possible GPIO pin configuration

// Callback function to be called when the timer expires
timer_t timer;
void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_set(LED_PIN);  // Set LED_PIN to turn on LED
    printf("LED is turned on\n");
}

int main(void) {
    // Initialize GPIO pin as output for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
timer.callback = timer_callback;
timer.arg = NULL;

    // Start a periodic timer with 1-second intervals
    int timer_duration = 1 * US_PER_SEC; // Define timer duration
    xtimer_set(&timer, timer_duration, timer_callback, NULL);

    // Enter an infinite loop (in a real application, there might be other logics)
    while (1) {
        // Do nothing; just wait for the timer
        xtimer_sleep(1); // Sleep for 1 second (or adapt to your needs)
    }
    return 0;
}