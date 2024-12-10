
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    // Initialize mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize event timer
    evtimer_init_msg(&evtimer);

    // Add periodic event
    evtimer_msg_event_t event;
    event.event.callback = periodic_callback;
    event.event.arg = NULL;
    event.event.interval = timex_set(0, PERIODIC_INTERVAL);
    event.event.next = timex_set(0, PERIODIC_INTERVAL);

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Message received\n");
            // Handle message
        }
    }

    return 0;
}
