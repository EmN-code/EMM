
#include <stdio.h>
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define LED0_PIN        GPIO_PIN(0, 13)  // Assuming LED is connected to GPIO 13 on ESP32
#define TIMER_INTERVAL  (1 * US_PER_SEC) // 1 second interval

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED0_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED0_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        // This loop can be used for other tasks if needed
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
