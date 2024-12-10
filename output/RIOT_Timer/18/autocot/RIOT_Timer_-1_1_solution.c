#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>

#define LED_PIN 2 // Assuming the LED is connected to GPIO2

// Function to initialize the LED
void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

// Function to turn on the LED
void led_on(void) {
    gpio_set(LED_PIN);
}

// Function to turn off the LED
void led_off(void) {
    gpio_clear(LED_PIN);
}

// Function that will be called when the timer expires
void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;
    if (led_state) {
        led_off();
    } else {
        led_on();
    }
    led_state = !led_state;
}

int main(void) {
    // Initialize the LED
    init_led();

    // Define a timer
    xtimer_t timer;

    // Set up the timer period to 1 second (1,000,000 microseconds)
    uint32_t interval = 1000000;

    // Initialize and start the timer to trigger every second
    xtimer_set(&timer, interval);
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Timer started, flipping LED every second...\n");

    // Wait indefinitely, let the timer handle LED toggling
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
