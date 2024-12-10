#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Ensure correct port and pin
#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

void led_on(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);
}

int main(void) {
    // Initialize the LED pin to output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a new timer
    xtimer_t timer;
    
    // Set up the timer with the interval and the callback
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        led_on(NULL);  // Turn LED on in the callback
        xtimer_sleep(1);  // Sleep for timer interval
    }
    return 0;
}