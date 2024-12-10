
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>

#define LED_PIN 2  // Assuming GPIO2 is connected to an LED on ESP32

static xtimer_t timer;
static bool led_state = false;

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Toggle LED state
    led_state = !led_state;
    printf("LED state: %s\n", led_state ? "ON" : "OFF");

    // Set the timer to trigger again in 1 second
    xtimer_set(&timer, 1 * MS_PER_SEC);
}

int main(void) {
    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the callback every 1 second
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, 1 * MS_PER_SEC);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_sleep(1);
    }

    return 0;
}
