
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "timex.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void send_event(void)
{
    evtimer_msg_event_t event;
    event.msg.content.value = 123; // Example content
    evtimer_add_msg(&evtimer, &event, thread_getpid());
}

void handle_message(msg_t *msg)
{
    printf("Received message with content: %d\n", msg->content.value);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer, &mbox);

    // Send an event every 5 seconds
    ztimer_set(ZTIMER_MSEC, (ztimer_now(ZTIMER_MSEC) + 5 * MS_PER_SEC), send_event);

    msg_t msg;
    while (1) {
        // Receive a message from the mailbox
        if (mbox_get(&mbox, &msg, 1) == 1) {
            handle_message(&msg);
        }
    }

    return 0;
}
