
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg.content.ptr = event;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Create an event for the timer
    evtimer_msg_event_t event;
    event.event.handler = _evtimer_mbox_handler;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Set the timer to trigger every second
    xtimer_set(&event.timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the message (e.g., toggle LED)
            printf("Timer triggered!\n");
        }
    }

    return 0;
}
