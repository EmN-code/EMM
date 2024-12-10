#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN        GPIO_PIN(0, 2)  // Assuming GPIO pin 2 on port 0 for the LED

static xtimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    puts("Starting the timer and LED toggle application...");

    // Initialize the GPIO pin for the LED as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error initializing GPIO pin!");
        return 1;
    }
    
    // Set up the xtimer
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}