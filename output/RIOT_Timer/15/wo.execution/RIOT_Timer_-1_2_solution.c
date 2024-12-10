#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Define the LED pin, assuming GPIO Port 0 Pin 2
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static void cb(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO pin\n");
        return 1;
    }

    printf("Starting timer example with LED toggle every second\n");

    // Set up the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        cb(NULL);
    }

    return 0; // Should never reach here
}