#include <ztimer.h>  // For ztimer functions
#include <stdio.h>    // For printf function
#include "board.h"  // For board-specific definitions
#include "periph/gpio.h" // For GPIO functions

#define INTERVAL (1U * MS_PER_SEC)  // Timer interval set to 1 second
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Change as per your board's GPIO setup

static ztimer_t timer;

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void) {
    printf("Starting the LED toggle timer example on ESP32\n");

    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    // Set up the timer
    ztimer_set(&ztimer_msec, &timer, INTERVAL, timer_callback, NULL);

    // Loop forever
    while (1) {
        // Use ztimer_periodic_wakeup to instantly reset the interval
        ztimer_periodic_wakeup(&ztimer_msec, &timer, INTERVAL);
    }

    return 0;
}