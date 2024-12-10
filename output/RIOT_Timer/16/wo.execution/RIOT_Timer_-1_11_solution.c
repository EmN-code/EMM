#include <ztimer.h>
#include <stdio.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <evtimer.h>
#include "periph/gpio.h"

#define INTERVAL 1U       // Timer interval in seconds.
#define LED_PIN GPIO_PIN(0) // Define the pin where the LED is connected.

// Function to toggle the LED state
void toggle_led(void)
{
    static bool led_state = false;
    led_state = !led_state;
    printf("LED is %s\n", led_state ? "ON" : "OFF");
    gpio_write(LED_PIN, led_state);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return -1;
    }

    evtimer_t evtimer;
    evtimer_event_t event;

    ztimer_clock_t *clock = ZTIMER_MSEC;

    // Initialize the event timer
    evtimer_init(&evtimer, clock);

    // Set up the event to trigger the toggle_led function
    evtimer_event_init(&event, toggle_led);

    // Add the event to the timer, setting it to trigger every INTERVAL seconds
    evtimer_add(&evtimer, &event, INTERVAL * MS_PER_SEC, EVTIMER_MODE_REPEAT);

    while (1) {
        // Main loop continues running, allowing evtimer to trigger
        ztimer_sleep(clock, 1);
    }

    return 0;
}