#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <msg.h>
#include <stdatomic.h>

// Define a periodic task structure
static ztimer_t timer;

// Define a struct for storing the event
typedef struct {
    ztimer_t timer;
    uint32_t interval;
    msg_t msg;
} event_periodic_t;

event_periodic_t periodic_event;

// Callback function that will be triggered by the timer
static void event_callback(void *arg)
{
    (void)arg;
    // For demonstration purposes, we'll toggle an LED or print something
    puts("Periodic event triggered.");
}

// Initialize the timer
void init_event_periodic(event_periodic_t *event, uint32_t interval)
{
    event->interval = interval;
    ztimer_set(&timer, ZTIMER_SEC, &event_callback, event->interval, 0);
}

int main(void)
{
    puts("Starting periodic event demo...");

    // Initialize the periodic event with a 1-second interval
    init_event_periodic(&periodic_event, 1);

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1); // Sleep to reduce CPU usage in demo
    }

    return 0;
}