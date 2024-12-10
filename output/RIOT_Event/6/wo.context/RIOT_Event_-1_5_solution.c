// Main function that demonstrates the use of periodic callback with event timers
#include <ztimer.h>
#include <event/callback.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    // Print a message every time the callback is triggered
    puts("Timer callback triggered!");
}

int main(void) {
    // Define an event timer
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init(&evtimer, ZTIMER_MSEC, &event);

    // Add the callback event to the timer
    evtimer_add(&evtimer, TIMER_INTERVAL);

    // Assign the callback function
    event.callback = timer_callback;
    event.arg = NULL;

    // Enter a loop to keep the main program running
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}