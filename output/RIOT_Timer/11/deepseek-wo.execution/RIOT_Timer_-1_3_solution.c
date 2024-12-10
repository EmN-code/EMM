
#include <stdlib.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define LED_PIN 2  // Assuming GPIO2 is connected to an LED on ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
