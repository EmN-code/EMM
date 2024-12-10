
#include "net/gcoap.h"
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
#include "net/sock/util.h"
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_mbox_event_t event;
static mbox_t mbox;
static msg_t msg_array[8];

static void _timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_array, ARRAY_SIZE(msg_array));

    // Initialize the mailbox
    mbox_init(&mbox, msg_array, ARRAY_SIZE(msg_array));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the event
    event.mbox = &mbox;
    event.event.callback = _timer_callback;
    event.event.arg = NULL;

    // Add the event to the timer
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &event.event.timer, TIMER_INTERVAL);

    // Main loop to receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
