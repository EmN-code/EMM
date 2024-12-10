
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_periodic_callback_t callback = {
        .handler = periodic_callback,
        .arg = NULL
    };

    event_periodic_init(&periodic_event, &callback);
    event_periodic_set_interval_ticks(&periodic_event, xtimer_ticks32(xtimer_ms_to_ticks32(INTERVAL_MS)));
    event_periodic_start(&periodic_event);

    while (1) {
        // Main loop
    }

    return 0;
}
