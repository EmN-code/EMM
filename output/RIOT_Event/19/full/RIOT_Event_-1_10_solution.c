#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"

static void callback_function(void *arg)
{
    (void)arg;
    printf("Callback triggered every second.\n");
}

int main(void)
{
    printf("Initializing event timer...\n");

    /* Create a periodic timer object */
    ztimer_t timer;
    ztimer_set_wakeup(&timer, 1000000UL, callback_function, NULL);

    /* Main loop, not exiting to keep the timer running */
    while (1) {
        ztimer_now(ZTIMER_MSEC);
    }

    return 0;
}