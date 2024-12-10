#include <stdio.h>
#include <event/callback.h>
#include <evtimer.h>

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

// Callback function that will be called every second
static void timer_callback(void *arg) {
    // Logic to turn on an LED
    printf("LED is turned ON\n");
    // Here you would typically toggle a GPIO pin connected to the LED
}

int main(void) {
    printf("Starting RIOT application on ESP32\n");

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Setup the event for the timer, triggering every 1 second
    evtimer_event.callback = timer_callback;
    evtimer_event.arg = NULL;
    evtimer_add(&evtimer, &evtimer_event, US_PER_SEC);

    // Enter in a loop to keep the main function running
    while (1) {
        evtimer_sleep(&evtimer);
    }

    return 0;
}