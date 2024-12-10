
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "ztimer.h"
#include "string.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "timex.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy value for the message
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer, event_callback, NULL);

    // Add an event to the event timer
    evtimer_msg_event_t event;
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to receive messages
    msg_t received_msg;
    while (1) {
        msg_receive(&received_msg);
        printf("Received message with value: %d\n", received_msg.content.value);
    }

    return 0;
}
