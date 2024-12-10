#include <xtimer.h>  // For timer functions
#include <stdio.h>   // For printf
#include <periph/gpio.h>  // For GPIO functions

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval in microseconds
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Define the GPIO pin where the LED is connected

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO pin\n");
        return -1;
    }

    // Timer setup
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        xtimer_sleep(TIMER_INTERVAL / US_PER_SEC);  // Convert microseconds to seconds for sleep
    }

    return 0;
}