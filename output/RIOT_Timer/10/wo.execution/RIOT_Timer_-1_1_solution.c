#include <timex.h>
#include <timers.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Example LED pin: GPIO Port 0, Pin 2

timer_t timer;

void callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    printf("Starting LED timer example on ESP32\n");

    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize GPIO\n");
        return 1;
    }

    // Initialize the timer
    timer_init(&timer, 1000000, callback, NULL);

    // Start the timer
    timer_set(&timer, 1000000, 1);

    // Keep the program running
    while (1) {
        // Let the timer interrupt the main loop
        xtimer_sleep(10);
    }

    return 0;
}