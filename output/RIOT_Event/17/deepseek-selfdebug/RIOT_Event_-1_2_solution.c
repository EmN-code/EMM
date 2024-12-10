
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

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

    // Keep the main thread alive
    while (1) {
        thread_sleep();
    }

    return 0;
}
