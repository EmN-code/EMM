#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Define the GPIO pin connected to the LED
#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval set to 1 second

static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: unable to initialize LED pin\n");
        return 1;
    }

    // Initialize timer struct
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Set timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}