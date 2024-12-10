#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "evtimer.h"
#include "dht_params.h"
#include "time_units.h"

#define MESSAGE_QUEUE_SIZE (8U)
#define EVENT_INTERVAL (1000U)  // 1000 ms

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static mbox_t mbox;

static void cb(void *arg)
{
    puts("Callback function called.");
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    // Initialize mbox
    mbox_init(&mbox, message_queue, MESSAGE_QUEUE_SIZE);

    // Initialize event timer with mbox handler
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    // Create mbox event
    evtimer_mbox_event_t event;
    event.event.callback = cb;
    event.event.period = EVENT_INTERVAL;

    // Add event to the event timer
    evtimer_add_mbox(&evtimer, &event, &mbox);

    unsigned interval = 500000;

    while(1){
        ztimer_sleep(ZTIMER_MSEC, interval);
    }
    
    return 0;
}