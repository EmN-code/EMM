
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    event_timeout_t timeout;

    // Initialize the event timeout structure
    event_timeout_init(&timeout, periodic_callback, NULL);

    // Set the timer to trigger the callback every second
    xtimer_set_timeout(&timer, &timeout, 1000000U);

    // Start the timer
    xtimer_set(&timer, 1000000U);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
