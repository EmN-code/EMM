
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the periodic callback
    xtimer_set_periodic(&timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the program running
    while (true) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
