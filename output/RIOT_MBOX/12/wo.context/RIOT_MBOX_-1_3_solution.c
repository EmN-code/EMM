#include "kernel_defines.h"
#include "kernel.h"
#include "evtimer.h"
#include "xtimer.h"

#define INTERVAL (1U * US_PER_SEC)

typedef struct {
    evtimer_event_t event;
    msg_t msg;
} message_box_event_t;

static void _timer_cb(void *arg)
{
    msg_t msg;
    message_box_event_t *event = (message_box_event_t *)arg;

    msg = event->msg;
    msg_try_send(&msg, thread_getpid(), 0);
    evtimer_add_msg((evtimer_t *)xtimer, &(event->event), INTERVAL);
}

int main(void)
{
    evtimer_t evtimer;
    message_box_event_t event;
    msg_init_queue(event.msg, 8);

    event.msg.type = 0; // Placeholder type

    evtimer_init_msg(&evtimer, _timer_cb, &event);
    evtimer_add_msg(&evtimer, &event.event, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle message
    }

    return 0;
}