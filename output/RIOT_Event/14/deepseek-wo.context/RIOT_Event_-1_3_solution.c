
#include <stdio.h>
#include "xtimer.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    // Start the timer with a 1 second interval
    xtimer_set(&timer, INTERVAL_MS * 1000);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
