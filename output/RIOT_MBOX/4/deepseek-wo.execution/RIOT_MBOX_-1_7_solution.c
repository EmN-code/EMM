
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    evtimer_msg_event_t *ev = (evtimer_msg_event_t *)event;
    msg_t msg;
    msg.content.ptr = (void *)ev;
    msg_send(&msg, ev->msg.sender_pid);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer.timer);

    kernel_pid_t pid = thread_getpid();

    evtimer_msg_event_t event;
    event.event.handler = _evtimer_mbox_handler;
    event.event.arg = NULL;

    evtimer_add_msg(&evtimer, &event, pid);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            evtimer_msg_event_t *received_event = (evtimer_msg_event_t *)msg.content.ptr;
            printf("Received event at %" PRIu32 "\n", xtimer_now_usec());
        }
    }

    return 0;
}
