
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    evtimer_add_msg(&evtimer, xtimer_ticks32(PERIODIC_INTERVAL), periodic_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_event_t *event = (evtimer_event_t *)msg.content.ptr;
            event->handler(event->arg);
            // Re-add the event to maintain periodicity
            evtimer_add_msg(&evtimer, xtimer_ticks32(PERIODIC_INTERVAL), periodic_callback, NULL);
        }
    }

    return 0;
}