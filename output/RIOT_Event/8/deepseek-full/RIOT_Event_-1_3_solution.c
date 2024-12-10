
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    evtimer_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;
    event.offset = 0;
    event.period = ZTIMER_MSEC(1000);

    evtimer_add_msg(&evtimer, &event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
