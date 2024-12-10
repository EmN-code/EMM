
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    // Simulate LED toggle by printing a message
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer);

    // Create an event for the timer
    evtimer_mbox_event_t event;
    event.event.callback = timer_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with the mailbox
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &event.event.timer, TIMER_INTERVAL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
