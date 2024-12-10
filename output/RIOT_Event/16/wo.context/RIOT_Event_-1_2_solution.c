#include <xtimer.h>
#include <stdio.h>
#include <event.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

static void _event_callback(void *arg)
{
    (void)arg;
    printf("Periodic Event Triggered!\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_periodic_init(ZTIMER_MSEC, &timer, TIMER_INTERVAL, _event_callback, NULL);
    while (1) {
        ztimer_periodic_wait(&timer);
    }
    return 0;
}