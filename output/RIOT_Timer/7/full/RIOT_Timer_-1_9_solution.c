#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(0, 2)  // Use a suitable GPIO pin for your board

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO pin!\n");
        return 1;
    }

    xtimer_set(&timer, INTERVAL, timer_callback, NULL);

    while(1) {
        // Busy wait loop, the timer interrupt will toggle the LED
    }

    return 0;
}