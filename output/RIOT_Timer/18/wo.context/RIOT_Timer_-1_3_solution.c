#include <xtimer.h>  // For timer functions
#include <stdio.h>   // For standard input/output
#include <stdlib.h>  // For standard library functions
#include <gpio.h>    // For GPIO pin functions
#include <board.h>   // For board specific definitions

// Define the pin where the LED is connected
#define LED_PIN GPIO_PIN(PORT_A, 5)

// Define the timer interval for 1 second (in microseconds)
#define TIMER_INTERVAL (1000000LU)

// Timer variable
static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);

    // Reset the timer
    xtimer_set(&timer, xtimer_usec_from_ticks(TIMER_INTERVAL));
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with the specified interval
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger for the first time
    xtimer_set(&timer, xtimer_usec_from_ticks(TIMER_INTERVAL));

    // Keep the program running
    while (1) {
        // Just a simple loop to keep the main thread alive
        xtimer_sleep(1);
    }

    return 0;
}
