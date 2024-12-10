
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "timex.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_mbox_t evtimer;

void send_event(void)
{
    msg_t msg;
    msg.content.value = 123; // Example event value
    evtimer_mbox_add_msg(&evtimer, &mbox, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_mbox_init(&evtimer, &mbox);

    // Schedule the first event
    evtimer_mbox_add_absolute(&evtimer, ztimer_now(ZTIMER_MSEC) + ZTIMER_SEC(1), send_event);

    while (1) {
        msg_t received_msg;
        if (evtimer_mbox_get(&evtimer, &received_msg, 1) == 1) {
            printf("Received event with value: %d\n", (int)received_msg.content.value);
        }

        // Schedule the next event after 1 second
        evtimer_mbox_add_absolute(&evtimer, ztimer_now(ZTIMER_MSEC) + ZTIMER_SEC(1), send_event);
    }

    return 0;
}
