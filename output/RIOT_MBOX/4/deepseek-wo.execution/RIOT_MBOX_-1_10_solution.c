
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x_regs.h"

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void _evtimer_mbox_handler(void *arg)
{
    evtimer_msg_event_t *event = (evtimer_msg_event_t *)arg;
    msg_t msg;
    msg.content.ptr = (void *)event;
    msg_send(&msg, event->msg.sender_pid);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer for message box events
    evtimer_init_mbox(&evtimer);

    // Create an event to be added to the event timer
    evtimer_msg_event_t event;
    event.event.handler = _evtimer_mbox_handler;
    event.event.arg = &event;

    // Add the event to the event timer with a target PID
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Set a timer to trigger the event after 1 second
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 1000);

    // Receive messages in a loop
    msg_t received_msg;
    while (1) {
        msg_receive(&received_msg);
        evtimer_msg_event_t *received_event = (evtimer_msg_event_t *)received_msg.content.ptr;
        printf("Received event from PID: %d\n", received_event->msg.sender_pid);
    }

    return 0;
}
