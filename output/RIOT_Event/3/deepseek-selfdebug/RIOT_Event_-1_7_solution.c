
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic_callback.h>

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_MSEC, &event_queue_null, periodic_callback, NULL);
    event_periodic_callback_start(&event, INTERVAL_MS);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
