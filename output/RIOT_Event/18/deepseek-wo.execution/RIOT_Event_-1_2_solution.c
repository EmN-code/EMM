
#include "thread.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "event/thread.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_mbox_t evtimer;
    evtimer_mbox_init(&evtimer, _main_msg_queue);

    ztimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &timer, MS_PER_SEC);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_mbox_event_handle(&evtimer);
        }
    }

    return 0;
}
