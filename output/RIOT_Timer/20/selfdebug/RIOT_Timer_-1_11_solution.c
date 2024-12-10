#include <xtimer.h>  // Include for using timers
#include <periph/gpio.h>  // Include for using GPIO peripherals

#define LED_PIN GPIO_PIN(PORT_A, 10)  // Define the LED pin, modify PORT_A to correct port

static xtimer_t timer;  // Declare a static timer

// Callback function for the timer
void timer_callback(void *arg) {
    // Turn on the LED
    gpio_write(LED_PIN, 1);
    
    // Set the timer again to trigger after 1 second
    xtimer_set(&timer, 1000000);
}

int main(void) {
    // Initialize the GPIO 
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with the callback function
    xtimer_init();

    // Ensure the LED is initially off
    gpio_write(LED_PIN, 0);

    // Set the timer to call the timer_callback after 1 second
    xtimer_set(&timer, 1000000);

    // Enter an infinite loop 
    while (1) {
        // Wait for interrupts
        xtimer_sleep(1);
    }

    return 0;
}