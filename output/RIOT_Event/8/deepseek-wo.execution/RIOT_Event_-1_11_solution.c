
#include <stdio.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the periodic callback every second
    xtimer_set_periodic(&timer, TIMER_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Do nothing, just keep the main thread alive
    }

    return 0;
}
