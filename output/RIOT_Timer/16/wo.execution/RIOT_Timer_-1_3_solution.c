#include <xtimer.h>
#include <ztimer.h>
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define INTERVAL (1U * US_PER_SEC) // interval for 1 second

// Define the LED pin for ESP32; use GPIO pin numbers
#define LED_PIN GPIO_PIN(PORT_A, 2)

static void timeout_cb(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer setup
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_USEC, INTERVAL, timeout_cb, NULL);

    // Start the timer to fire every second
    ztimer_start(&timer);

    // Main loop
    while (1) {
        // Putting the CPU to low power mode and wait for interrupt
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return 0;
}