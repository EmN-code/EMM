
#include <stdio.h>
#include <unistd.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_array[1];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the periodic event
    evtimer_event_t event;
    event.event.offset = 0;
    event.event.interval = PERIODIC_INTERVAL;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer
    evtimer_add_event(&evtimer, &event);

    // Main loop to handle events
    while (1) {
        msg_receive(msg_array, 1);
        evtimer_event_handle(&evtimer);
    }

    return 0;
}
