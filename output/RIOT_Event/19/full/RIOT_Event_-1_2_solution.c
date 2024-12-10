#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1LU * US_PER_SEC) // Interval is 1 second

static void event_periodic(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic event triggered\n");
}

int main(void)
{
    ztimer_t timer;
    timer.callback = event_periodic;
    timer.arg = NULL;

    // Set the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    while (1) {
        // Let the event loop take over
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}