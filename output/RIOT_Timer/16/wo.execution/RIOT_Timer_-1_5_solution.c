#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"

#define LED_PIN GPIO_PIN(0, 2)      // Assumption: Set the correct port and pin for the LED
#define INTERVAL (1U * US_PER_SEC)  // Interval for 1 second

static void timer_callback(void)
{
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Ensure to initialize the LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.interval = INTERVAL;
    timer.arg = NULL;

    // Set and start the timer
    ztimer_set(&timer, INTERVAL);

    // Keeping the main function alive
    while (1) {
        xtimer_sleep(1); // Sleep for a while to let the timer callback run
    }

    return 0;
}