
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

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer with the mailbox handler
    evtimer_init_mbox(&evtimer);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, &mbox, (msg_t *)periodic_callback, NULL, PERIODIC_INTERVAL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, TIMEOUT_NEVER) == 0) {
            ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
        }
    }

    return 0;
}
