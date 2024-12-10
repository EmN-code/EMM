
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MSG_QUEUE_SIZE (16)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer for message events
    evtimer_init_msg(&evtimer);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, 1 * US_PER_SEC, periodic_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the received message here
    }

    return 0;
}
