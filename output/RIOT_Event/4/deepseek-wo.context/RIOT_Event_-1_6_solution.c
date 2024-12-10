
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;
    event_periodic_init(&periodic_event.periodic, xtimer_now_usec64(), INTERVAL_MS * 1000, periodic_callback, NULL);
    event_callback_init(&periodic_event.event, periodic_callback, NULL);

    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process();
    }

    return 0;
}
