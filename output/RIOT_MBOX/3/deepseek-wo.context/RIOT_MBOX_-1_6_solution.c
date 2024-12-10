
#include "thread.h"
#include "msg.h"
#include "evtimer.h"
#include "ztimer.h"
#include "event.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t _evtimer;
static evtimer_msg_t _evtimer_msg;

static void _event_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init(&_evtimer);

    _evtimer_msg.event.handler = _event_handler;
    _evtimer_msg.event.arg = NULL;

    evtimer_add_msg(&_evtimer, ZTIMER_MSEC, 1000, &_evtimer_msg, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
