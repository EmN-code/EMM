#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_GPIO_PIN      GPIO_PIN(PORT_C, 8)  // Define the GPIO pin for the LED
#define TIMER_INTERVAL    (1U * US_PER_SEC)    // Set timer interval to 1 second

static xtimer_t timer;

// Timer callback function
static void timer_callback(void *arg) {
    // Toggle the LED state
    (void) arg;
    gpio_toggle(LED_GPIO_PIN);
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_GPIO_PIN, GPIO_OUT)) {
        puts("Error: unable to initialize LED GPIO pin\n");
        return 1;
    }

    // Set up the timer with the callback function
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    if (xtimer_set(&timer, TIMER_INTERVAL) != 0) {
        puts("Error: unable to set up the timer\n");
        return 1;
    }

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}