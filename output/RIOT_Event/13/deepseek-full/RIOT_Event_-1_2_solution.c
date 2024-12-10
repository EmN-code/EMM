
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t periodic_msg;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the periodic event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic message
    periodic_msg.content.ptr = NULL;
    periodic_msg.type = 0;

    // Add the periodic event to the timer
    evtimer_add_msg(&evtimer, &periodic_msg, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
