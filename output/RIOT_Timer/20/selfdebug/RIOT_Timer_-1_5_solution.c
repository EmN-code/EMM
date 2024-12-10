#include <xtimer.h>
#include <stdio.h>
#include <timex.h>
#include <gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0) // Correct the GPIO Port for ESP32
#define INTERVAL (1U * US_PER_SEC)

static xtimer_t timer;

// Timer callback to turn on the LED
void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggles the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer to expire every second
    xtimer_set(&timer, INTERVAL);

    while (1) {
        // Code can perform other tasks here
        xtimer_sleep(1); // Sleep to simulate doing other work
    }

    return 0;
}