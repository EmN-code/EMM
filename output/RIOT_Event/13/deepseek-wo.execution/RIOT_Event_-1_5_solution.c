
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the periodic callback to trigger every second
    xtimer_set_periodic(&timer, TIMER_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Do nothing, just keep the main thread alive
    }

    return 0;
}
