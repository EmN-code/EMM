
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
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer with the message handler
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    evtimer_add_msg(&evtimer, timex_set(PERIODIC_INTERVAL, 0), periodic_callback, NULL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
