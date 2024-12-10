
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <thread.h>
#include <ztimer.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <timex.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic timer
    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_MSEC, periodic_callback, NULL);

    // Set the timer to trigger every second
    ztimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
