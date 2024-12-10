#include <timex.h>
#include <thread.h>
#include <periph/gpio.h>

#define LED_PIN 2  // Define the LED pin

tim_t timer_dev;

// Callback function that gets called every second
timer_cb_t timer_callback(void *arg) {
    (void)arg;  // Unused parameter, avoid compiler warning
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    return 0;
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    if (timer_init(&timer_dev, 1, timer_callback, NULL) != 0) {
        // Timer initialization failed
        return 1;
    }

    // Start the timer with a one-second interval
    timer_start(&timer_dev);

    // Thread loop (usually you would have other code here)
    while (1) {
        thread_yield();  // Yield to lower priority threads
    }

    return 0;
}