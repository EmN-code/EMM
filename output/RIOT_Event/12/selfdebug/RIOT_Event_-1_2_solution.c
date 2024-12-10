#include <ztimer.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)

static void on_timeout(void *arg)
{
    (void)arg;
    printf("Timer callback fired!\n");
}

int main(void)
{
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, on_timeout, NULL, INTERVAL);
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        // Main loop does nothing, just keep the application alive
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}