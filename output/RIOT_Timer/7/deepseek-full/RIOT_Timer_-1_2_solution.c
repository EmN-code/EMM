
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define LED_PIN             GPIO_PIN(0, 13)  // Example GPIO pin for LED
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        // This loop can be used for additional tasks if needed
    }

    return 0;
}
