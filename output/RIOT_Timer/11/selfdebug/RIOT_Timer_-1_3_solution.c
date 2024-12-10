// Remember to include necessary header files in the correct order
#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

// Define the LED pin (assuming GPIO_PIN function is available)
#define LED_PIN GPIO_PIN(0, 2) // Adjust as per your board configuration

// Define the timer interval (1 second in microseconds)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Timer object
static ztimer_t timer;

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;
    // Toggle LED state
    gpio_write(LED_PIN, led_state);
    led_state = !led_state;
    // Re-arm the timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop does nothing, the timer runs in the background
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}