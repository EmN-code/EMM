#include <stdio.h>
#include <xtimer.h>
#include <event.h> // Ensure you include the event module and initialization functions

#define INTERVAL (1U * US_PER_SEC) // 1 second converted to microseconds

static void event_callback(void *arg) {
    (void)arg; // Not used
    printf("Event trigger: Executing periodic callback...\n");
}

int main(void) {
    puts("Initializing and starting the periodic event...");

    // Initialize a timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        event_callback(NULL); // Call the event callback directly
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}