
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic_callback.h>

#define PERIOD_US (1000000U)  // 1 second in microseconds

static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, xtimer_ticks32(PERIOD_US), periodic_callback, NULL);

    // Set the period for the periodic event
    event_periodic_callback_set_period(&periodic_event, xtimer_ticks32(PERIOD_US));

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
