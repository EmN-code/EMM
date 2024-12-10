
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue and mailbox
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    ztimer_t periodic_event;
    periodic_event.callback = periodic_callback;
    periodic_event.arg = NULL;
    periodic_event.offset = 0;
    periodic_event.period = PERIODIC_INTERVAL;

    // Add the periodic event to the event timer
    evtimer_add_msg(&evtimer, &periodic_event);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
