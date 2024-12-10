#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)

void timer_callback(void *arg) {
    (void)arg;
    // Turn on the LED
    gpio_set(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Declare and configure a ztimer for 1 second intervals
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, 1000);

    // Set the timer callback function
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    ztimer_start(&timer);

    while (1) {
        // Keep the main function running
    }

    return 0;
}