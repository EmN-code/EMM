
#include "xtimer.h"
#include "timex.h"
#include "kernel_defines.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg_receive(&msg);
    // Simulate turning on LED
    printf("LED turned on\n");
}

int main(void)
{
    // Initialize mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize event timer
    evtimer_init(&evtimer, _evtimer_mbox_handler);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, &mbox, NULL, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Simulate other tasks
        xtimer_sleep(1);
    }

    return 0;
}
