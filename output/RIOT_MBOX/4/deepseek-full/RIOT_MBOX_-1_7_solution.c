
#include "evtimer_msg.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "ztimer.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "xtimer.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_handler(void *arg)
{
    msg_t msg;
    msg.content.ptr = "Event Triggered";
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer, event_handler, NULL);

    // Add an event to the event timer
    evtimer_event_t event;
    evtimer_add(&evtimer, &event, 1000000); // 1 second interval

    while (1) {
        msg_t received_msg;
        mbox_get(&mbox, &received_msg, 1); // Blocking call to receive message
        printf("Received message: %s\n", (char *)received_msg.content.ptr);
    }

    return 0;
}
