
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <timex.h>
#include <xtimer.h>
#include <thread.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer to trigger the periodic callback every second
    xtimer_init(&timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the timer
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Yield to allow other threads to run
    thread_yield();

    return 0;
}
