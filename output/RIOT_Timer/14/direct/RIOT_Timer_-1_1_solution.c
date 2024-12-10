#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected to GPIO2
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void timer_callback(void) {
    static int led_state = 0;
    
    // Toggle the LED state
    if (led_state) {
        gpio_clear(LED_PIN);
        printf("LED off\n");
    } else {
        gpio_set(LED_PIN);
        printf("LED on\n");
    }
    led_state = !led_state;
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing LED pin\n");
        return 1;
    }

    // Create a timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        // Wait for the interval period
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        // Call the timer callback function
        timer_callback();
    }
    
    return 0;
}