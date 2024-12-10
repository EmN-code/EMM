#include <stdio.h> 
#include "xtimer.h" 
#include "board.h" 
#include "periph/gpio.h" 

// Set the interval to 1 second (1,000,000 microseconds)
#define INTERVAL (1000000U)

// LED pin definition for ESP32
#define LED_PIN GPIO_PIN(PORT_B, 0)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    if (gpio_read(LED_PIN)) {
        gpio_clear(LED_PIN);  // Turn off LED
    } else {
        gpio_set(LED_PIN);    // Turn on LED
    }
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Declare a timer variable
    xtimer_ticks32_t last_wakeup = xtimer_now();

    printf("Starting timer example with LED...");

    while (1) {
        // Wait until the next interval
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        // Call the timer callback
        timer_callback(NULL);
    }
    return 0;
}