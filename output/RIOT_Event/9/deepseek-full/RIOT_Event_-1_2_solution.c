
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "unistd.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_mbox_t mbox;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer with the mailbox handler
    evtimer_init_mbox(&evtimer);

    // Initialize the mailbox
    evtimer_mbox_init(&mbox, &evtimer);

    // Set up the periodic event
    evtimer_mbox_add_msg(&mbox, periodic_callback, NULL, PERIODIC_INTERVAL);

    // Main loop to handle events
    while (1) {
        evtimer_mbox_event_t event;
        if (evtimer_mbox_get(&mbox, &event, TIME_TICKS_PER_SEC)) {
            event.handler(event.arg);
        }
    }

    return 0;
}
