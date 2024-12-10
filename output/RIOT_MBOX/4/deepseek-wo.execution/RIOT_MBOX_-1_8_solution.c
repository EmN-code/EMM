
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <mbox.h>
#include <mma8x5x_regs.h>

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg.content.ptr = event;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = _evtimer_mbox_handler;
    event.event.arg = NULL;

    ztimer_now_t now = ztimer_now(ZTIMER_MSEC);
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 1000);

    while (1) {
        msg_t received_msg;
        msg_receive(&received_msg);
        evtimer_msg_event_t *received_event = (evtimer_msg_event_t *)received_msg.content.ptr;
        printf("Event received!\n");
    }

    return 0;
}
