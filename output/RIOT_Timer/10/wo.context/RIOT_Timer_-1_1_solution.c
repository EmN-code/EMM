#include "tim.h"
#include "periph/gpio.h"

#define LED_PIN     GPIO_PIN(x) // Use the correct pin number connected to the LED
#define TIMER_FREQUENCY    (1000000)  // Timer frequency for 1-second interval

static void timer_callback(void *arg)
{
    (void)arg;  // Unused
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the GPIO pin connected to the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer
    tim_t timer = TIMER_DEV(0); // Use the first available timer (e.g., TIMER_DEV(0))
    timer_init(timer, TIMER_FREQUENCY, timer_callback, NULL);

    // Start the timer to trigger every second
    timer_start(timer);

    // Main loop to keep the program running
    while (1) {
        thread_sleep(); // Let the main thread sleep
    }

    return 0;
}