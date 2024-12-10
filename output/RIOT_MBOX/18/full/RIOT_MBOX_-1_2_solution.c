#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "evtimer.h"

#define INTERVAL_MS 1000

static msg_t queue[8];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_msg_event_t event;

void evtimer_callback(void *arg)
{
    msg_t msg;
    msg_receive(&msg);
    printf("Event received!\n");
}

int main(void)
{
    msg_init_queue(queue, 8);
    mbox_init(&mbox, queue, 8);

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, event);

    ztimer_t timer;
    ztimer_set_wd(&timer, ZTIMER_MSEC, INTERVAL_MS);
    ztimer_start(&timer);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        evtimer_callback(NULL);
    }
    return 0;
}