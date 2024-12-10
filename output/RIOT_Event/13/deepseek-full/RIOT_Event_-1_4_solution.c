
#include "xtimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "timex.h"
#include "kernel_defines.h"
#include "thread.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define PERIODIC_EVENT_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, ARRAY_SIZE(msg_array));

    // Initialize the event timer with the mailbox
    evtimer_init_msg(&evtimer, &mbox);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, &periodic_callback, NULL, xtimer_ticks32(PERIODIC_EVENT_INTERVAL));

    msg_t msg;
    while (1) {
        // Wait for a message in the mailbox
        mbox_get(&mbox, &msg);

        // Handle the message (in this case, just call the callback)
        ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
