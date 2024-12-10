
#include "thread.h"
#include "xtimer.h"
#include "event/thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    // Your periodic task code here
}

int main(void)
{
    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
