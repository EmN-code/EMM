
#include <stdlib.h>
#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1 * ZTIMER_SEC)

void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event callback triggered!\n");
}

int main(void) {
    ztimer_t timer;

    // Initialize the ztimer
    ztimer_init(&timer);

    // Set the periodic callback function to be called every second
    ztimer_set(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
