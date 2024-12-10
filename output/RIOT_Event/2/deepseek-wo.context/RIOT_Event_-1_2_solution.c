
#include "event/periodic.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"

#define INTERVAL_US (1 * US_PER_SEC)

static void callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t *queue = &event_queue_syscalls;

    event_periodic_callback_init(&event, clock, queue, callback, NULL);
    event_periodic_callback_start(&event, INTERVAL_US);

    while (1) {
        thread_yield();
    }

    return 0;
}
