#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "stdio.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)

static void _callback(void *arg)
{
    (void)arg;
    puts("Event triggered every second!");
}

int main(void)
{
    static msg_t msg_queue[8];
    evtimer_mbox_t evtimer;
    evtimer_mbox_event_t event;
    mbox_t mbox;
    msg_init_queue(msg_queue, 8);      // Initialize the message queue
    mbox_init(&mbox, msg_queue, 8);   // Initialize the mailbox

    evtimer_mbox_init(&evtimer, &mbox); // Initialize the evtimer for the mailbox
    evtimer_mbox_event_init(&event, _callback, NULL); // Initialize the event

    evtimer_mbox_add(&evtimer, &event, INTERVAL); // Add the event to the timer with the specified interval

    evtimer_mbox_run(&evtimer); // Run the evtimer

    return 0;
}