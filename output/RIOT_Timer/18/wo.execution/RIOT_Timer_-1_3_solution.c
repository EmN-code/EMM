#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval in microseconds
#define LED_PIN GPIO_PIN(PORT_A, 0)  // assuming LED is connected to PORT_A, pin 0

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // toggle the state of the LED
    xtimer_set(&timer, TIMER_INTERVAL);  // reset the timer for the next interval
}

int main(void) {
    puts("RIOT Timer Example: LED blinker");

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: Unable to initialize LED pin");
        return 1;
    }

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the initial timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Sleep to save CPU cycles; ztimer_sleep no longer used since we rely on timer callbacks
        xtimer_sleep(1);
    }

    return 0;
}