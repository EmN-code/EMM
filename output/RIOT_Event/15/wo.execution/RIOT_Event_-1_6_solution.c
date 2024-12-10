#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "event/thread.h"
#include "event/periodic.h"

#define INTERVAL (1LU * US_PER_SEC)

static ztimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Periodic callback every second");
}

int main(void)
{
    puts("Initializing periodic event");

    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.interval = INTERVAL;

    ztimer_periodic_init(ZTIMER_SEC, &timer, INTERVAL, timer.callback, timer.arg);
    ztimer_periodic_start(&timer);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }
    return 0;
}