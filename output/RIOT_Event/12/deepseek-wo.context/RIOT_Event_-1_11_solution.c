
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    event_callback_t event_cb;

    // Initialize the event callback structure
    event_cb.handler = periodic_callback;
    event_cb.arg = NULL;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set_periodic(&timer, INTERVAL_MS, (xtimer_callback_t)event_cb.handler, event_cb.arg);

    // Main loop to keep the application running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
