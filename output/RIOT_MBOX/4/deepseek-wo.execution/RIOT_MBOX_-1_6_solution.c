
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <mbox.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void event_handler(void *arg)
{
    evtimer_msg_event_t *event = (evtimer_msg_event_t *)arg;
    msg_t msg;
    msg.content.ptr = event;
    msg.type = 0;
    msg_send(&msg, event->msg.sender_pid);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = event_handler;
    event.event.arg = &event;

    kernel_pid_t pid = thread_getpid();

    while (1) {
        evtimer_add_msg(&evtimer, &event, pid);
        xtimer_sleep(5);
    }

    return 0;
}
