
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define LED_PIN 5  // Assuming LED is connected to GPIO 5 on ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        // Main loop to keep the program running
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
