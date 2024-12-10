#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define PERIODIC_INTERVAL (1LU * US_PER_SEC)

static void _event_handler(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    ztimer_t periodic_timer;
    periodic_timer.callback = _event_handler;
    periodic_timer.arg = NULL;
    ztimer_init(ZTIMER_MSEC);
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, PERIODIC_INTERVAL);
        _event_handler(NULL);
    }
    return 0;
}