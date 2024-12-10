#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <stdio.h>
#include <stdlib.h>
#include <event/callback.h>
#include <event/timeout.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <unistd.h>

#define INTERVAL (1U * US_PER_SEC)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Callback executed!\n");
}

int main(void)
{
    printf("Periodic event example\n");

    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC);
    ztimer_set(&timer, INTERVAL, periodic_callback, NULL);

    while (1) {
        sleep(1);
    }

    return 0;
}