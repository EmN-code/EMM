
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "unistd.h"
#include "stdbool.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_mbox_event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the periodic event
    evtimer_mbox_init_msg(&periodic_event, &mbox, periodic_callback, NULL);

    // Add the periodic event to the timer
    evtimer_mbox_add_msg(&periodic_event, (xtimer_ticks32(PERIODIC_INTERVAL)));

    // Main loop to process messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        msg_receive(&msg);
    }

    return 0;
}
