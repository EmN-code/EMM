
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "time_units.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static evtimer_msg_t periodic_event;
static mbox_t periodic_mbox;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the message box
    mbox_init(&periodic_mbox, NULL, 0);

    // Initialize the event timer
    evtimer_init_mbox(&periodic_timer, &periodic_mbox);

    // Set up the periodic event
    periodic_event.event.handler = (event_handler_t)periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add_msg(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Start the periodic event
    evtimer_add_msg(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        mbox_get(&periodic_mbox, &msg);
        event_t *event = (event_t *)msg.content.ptr;
        event->handler(event->arg);
    }

    return 0;
}
