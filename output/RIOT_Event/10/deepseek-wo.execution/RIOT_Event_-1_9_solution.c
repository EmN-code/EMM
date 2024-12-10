
#include "xtimer.h"
#include "msg.h"
#include "evtimer_msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;
    event.event.interval = PERIODIC_INTERVAL;
    event.event.next = 0;

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_t *event = (evtimer_msg_event_t *)msg.content.ptr;
            event->event.handler(event->event.arg);
            evtimer_add_msg(&evtimer, event, thread_getpid());
        }
    }

    return 0;
}
