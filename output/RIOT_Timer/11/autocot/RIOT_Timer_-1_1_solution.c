#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timex.h"
#include "timers.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Define your LED pin here (e.g., GPIO 0, Pin 2)
#define TIMER_FREQ (1U) // Timer frequency: 1 Hz

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    printf("Timer triggered, LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer configuration
    tim_t timer = TIMER_DEV(0); // Use timer 0
    timer_init(timer, TIMER_FREQ, timer_callback, NULL);
    timer_start(timer);

    puts("Timer setup complete, LED should toggle every second.\n");

    // Keeping the main function alive
    while (1) {
        // Could put the CPU to sleep for low power consumption
    }

    return 0;
}