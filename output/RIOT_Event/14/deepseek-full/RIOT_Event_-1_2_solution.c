
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_mbox_t mbox;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic event triggered!");
}

int main(void)
{
    // Initialize the event timer with the mbox handler
    evtimer_init_mbox(&evtimer);

    // Create a periodic event
    evtimer_event_t event;
    event.event.handler = event_periodic_callback;
    event.event.arg = NULL;

    // Add the periodic event to the event timer
    evtimer_add_event(&evtimer, &event, PERIODIC_INTERVAL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        msg_receive(&mbox.mbox, &msg);
        evtimer_event_t *event = (evtimer_event_t *)msg.content.ptr;
        event->event.handler(event->event.arg);
    }

    return 0;
}
