
#include <stdio.h>
#include <stdint.h>
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "mbox.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void event_callback(void *arg)
{
    (void)arg;
    static uint32_t event_count = 0;
    msg_t msg;
    msg.content.value = event_count++;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox with the correct number of arguments
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Set up the event timer to trigger every 1 second
    evtimer_mbox_t evtimer;
    evtimer_mbox_init(&evtimer, &mbox);
    evtimer_mbox_add_msg(&evtimer, ZTIMER_USEC, timex_set(1, 0), event_callback, NULL);

    // Main loop to handle incoming messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received event with count: %lu\n", (unsigned long)msg.content.value);
        }
    }

    return 0;
}
