#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>
#include "evtimer_mbox.h"

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

void toggle_led(void) {
    // Simulate LED toggle function since gpio functions are not permitted
    printf("LED Toggled\n");
}

void timer_event_handler(void *arg)
{
    (void)arg;
    toggle_led();
}

int main(void) {
    // Initialize evtimer for mbox events
    evtimer_init_mbox(&evtimer);

    // Setup the event
    evtimer_event.event.callback = timer_event_handler;
    evtimer_event.event.arg = NULL; // No additional arguments required

    // Add the event to the timer with a one-second timeout repeatedly
    while (1) {
        int res = evtimer_add(&evtimer, &evtimer_event.event);
        if (res != 0) {
            printf("Error setting timer. Retrying...\n");
            usleep(1000000); // wait a second
            continue;
        }
        usleep(1000000); // Wait to simulate a repeating 1-second timer
    }
    return 0;
}