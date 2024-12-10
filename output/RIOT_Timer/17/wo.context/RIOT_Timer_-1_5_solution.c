#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC)  // 1-second interval
#define LED_PIN GPIO_PIN(PORT_A, 0) // Use the appropriate port and pin

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    xtimer_set(&timer, INTERVAL);  // Reschedule timer
}

int main(void) {
    // Initialize the LED pin
    if(gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    // Set up the timer with the callback
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the periodic timer
    xtimer_set(&timer, INTERVAL);

    // Main thread does nothing, careful with excessive idle power consumption
    while (1) {
        xtimer_sleep(10);  // Just sleep the main thread
    }

    return 0;
}