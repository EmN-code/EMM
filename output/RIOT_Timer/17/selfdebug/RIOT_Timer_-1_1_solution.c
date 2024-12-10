#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define the LED pin, adjust PORT_A and 5 based on your hardware configuration
#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval set to one second in microseconds

static void timer_cb(void *arg) {
    (void)arg;
    static bool led_state = false;  // Track the LED state
    led_state = !led_state;  // Toggle the LED state
    gpio_write(LED_PIN, led_state);  // Write the new state to the LED
}

int main(void) {
    // Initialize the GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        // Error handling if GPIO initialization fails
        return -1;
    }
    
    // Initialize the timer to call 'timer_cb' every TIMER_INTERVAL microseconds
    ztimer_periodic_t timer;
    ztimer_periodic_init(ZTIMER_SEC, &timer, timer_cb, NULL, TIMER_INTERVAL);
    
    // Start the timer
    ztimer_periodic_start(&timer);

    // Main loop, nothing else to do as the operation is interrupt driven
    while (1) {
        // Put MCU to sleep to save power when not processing anything
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}