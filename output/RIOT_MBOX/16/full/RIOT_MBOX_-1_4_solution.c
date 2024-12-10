#include "mma8x5x_params.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "evtimer_mbox.h"
#include "evtimer.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "msg.h"
#include "xtimer.h"
#include "stdio.h"
#include "ztimer.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"

#define QUEUE_SIZE 8
#define EVENT_INTERVAL (2 * US_PER_SEC)

static msg_t _msg_queue[QUEUE_SIZE];

void event_callback(event_t *event)
{
    msg_t msg;
    while (true) {
        if (msg_receive(&msg) >= 0) {
            printf("Received event with value: %lu\n", msg.content.value);
        }
    }
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_mbox_event_t mbox_event;
    mbox_t mbox;
    msg_t mbox_buffer[QUEUE_SIZE];

    msg_init_queue(_msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, mbox_buffer, ARRAY_SIZE(mbox_buffer));
    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    timex_t interval = timex_set(EVENT_INTERVAL, 0);

    while (true) {
        mbox_event.event.period = interval; // Set the period for timed events
        mbox_event.event.callback = event_callback;
        evtimer_add(&evtimer, &mbox_event.event);

        ztimer_sleep(ZTIMER_USEC, EVENT_INTERVAL);
    }

    return 0;
}