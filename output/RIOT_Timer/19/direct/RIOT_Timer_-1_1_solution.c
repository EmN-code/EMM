#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO pin\n");
        return 1;
    }

    printf("Starting timer to trigger every second\n");

    // Create the timer
    xtimer_t timer;
    timer.callback = timer_callback;

    while (1) {
        // Set the timer to trigger every second
        xtimer_set(&timer, INTERVAL);
        // Sleep the main thread to allow time for the timer thread to execute
        xtimer_usleep(INTERVAL);
    }

    return 0;
}