#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Assume LED is connected on port 0, pin 2
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static xtimer_ticks32_t last_wakeup;

void timer_callback(void)
{
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    
    // Schedule the next wakeup
    last_wakeup = xtimer_now();
    xtimer_set(&last_wakeup, xtimer_usec_from_sec(INTERVAL));

    printf("LED toggled!\n");
}

int main(void)
{
    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    // Set the initial state of the LED to OFF
    gpio_clear(LED_PIN);

    // Set up the initial wakeup time
    last_wakeup = xtimer_now();
    
    // Enter a loop to repeatedly call the callback
    while (1) {
        timer_callback();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}