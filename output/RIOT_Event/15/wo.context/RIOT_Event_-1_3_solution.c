#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include "xtimer.h"

#define INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer callback executed");
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        /* Sleep for INTERVAL microseconds */
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

        /* Execute the callback function */
        timer_callback(NULL);
    }

    return 0;
}