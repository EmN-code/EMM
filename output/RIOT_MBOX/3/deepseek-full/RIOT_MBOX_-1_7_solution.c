
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "timex.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void _evtimer_msg_handler(evtimer_event_t *event)
{
    msg_t msg;
    msg.content.ptr = event;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create an event to be added to the event timer
    evtimer_mbox_event_t event;
    event.mbox = &mbox;

    // Add the event to the event timer with a 1-second interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to receive messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}
