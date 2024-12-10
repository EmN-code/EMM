#include <timex.h>  // For timex structure and timer functions
#include <timer.h>  // Basic timer management
#include <stdio.h>  // For printf function
#include <periph/gpio.h>  // For GPIO functionality

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define LED pin, PORT_A and pin 5 as an example

void timer_callback(void *arg) {
    (void)arg;  // unused parameter
    // Toggle the state of the LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: failed to initialize LED_PIN\n");
        return 1;
    }
    printf("Initialized LED_PIN as output.\n");

    // Set up a timer that triggers every second
    tim_t timer;
    timex_t interval = {1, 0};  // 1 second interval
    if (timer_init(&timer, interval)) {
        printf("Error: failed to initialize the timer\n");
        return 1;
    }
    timer_set_callback(&timer, &timer_callback, NULL);
    printf("Timer set to trigger every second.\n");

    while (1) {
        // Let the main function keep running indefinitely
        // Other tasks can be added here
    }

    return 0;
}