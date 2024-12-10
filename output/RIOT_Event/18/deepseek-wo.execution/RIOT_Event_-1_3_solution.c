
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer, periodic_callback, NULL);
    xtimer_periodic_event_oneshot(&evtimer.timer, xtimer_ticks_from_usec(1000000));

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_handle(&evtimer, &msg);
        }
    }

    return 0;
}
