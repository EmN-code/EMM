
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the periodic callback every second
    xtimer_set(&timer, TIMER_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
