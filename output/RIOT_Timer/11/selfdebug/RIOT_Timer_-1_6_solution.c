#include <xtimer.h>  // Timer functions for sleep and interaction with timers
#include <stdio.h>    // Standard IO for debugging and logging
#include <periph/gpio.h>  // GPIO functions to manipulate LEDs

#define GPIO_PIN(x, y) ((gpio_t)((x * 16) + (y)))  // Define GPIO_PINA
#define US_PER_SEC (1000000U)  // Define microseconds per second constant

void timer_callback(void *arg){
    (void)arg;  // To avoid unused variable warning
    gpio_toggle(GPIO_PIN(0, 0));  // Toggle LED on pin A0
}

int main(void) {
    printf("Starting ESP32 timer example...\n");

    // Initialize GPIO pin for LED
    if (gpio_init(GPIO_PIN(0, 0), GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;
    }

    // Setting up a timer that triggers every second
    xtimer_ticks32_t interval = { .ticks32 = US_PER_SEC };
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, interval);  // Sleep for 1 second
        timer_callback(NULL);  // Execute callback
    }

    return 0;
}