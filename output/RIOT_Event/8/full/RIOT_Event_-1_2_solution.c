#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "event/callback.h"
#include <errno.h>

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event callback triggered\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC, &timer);
    ztimer_set(&timer, 1000, event_periodic_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}