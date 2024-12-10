
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <time_units.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <event/thread.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        periodic_callback(NULL);
        thread_yield();
    }

    return 0;
}
