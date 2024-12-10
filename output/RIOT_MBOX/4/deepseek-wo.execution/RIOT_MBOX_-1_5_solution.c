
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <mbox.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg.content.ptr = event;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer);

    // Add a message to the event timer to be sent at intervals
    msg_t msg;
    msg.content.ptr = NULL;
    evtimer_add_msg(&evtimer, &msg, ZTIMER_TICKS(ZTIMER_USEC, 1000000));

    // Main loop to handle messages from the mailbox
    while (1) {
        msg_t received_msg;
        mbox_get(&mbox, &received_msg, 1); // Blocking call to get message
        printf("Message received!\n");
    }

    return 0;
}
