
#include "thread.h"
#include "msg.h"
#include "evtimer.h"
#include "ztimer.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;

static void _evtimer_mbox_handler(event_t *event)
{
    evtimer_mbox_event_t *ev = (evtimer_mbox_event_t *)event;
    msg_t msg;
    msg.content.ptr = (char *)"Event received";
    msg_send(&msg, ev->mbox);
}

static void _event_loop(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init_mbox(&evtimer);
    evtimer_event.event.handler = _evtimer_mbox_handler;

    evtimer_add_mbox(&evtimer, &evtimer_event, thread_getpid());

    ztimer_set(ZTIMER_MSEC, &evtimer_event.event.timer, 1000);

    _event_loop();

    return 0;
}
