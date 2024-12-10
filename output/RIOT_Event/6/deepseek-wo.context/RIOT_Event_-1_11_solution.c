
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_msg_event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init(&evtimer);

    periodic_event.event.offset = 0;
    periodic_event.event.interval = 1 * US_PER_SEC;
    periodic_event.event.next = NULL;
    periodic_event.event.msg.content.ptr = NULL;
    periodic_event.event.msg.type = 0;
    periodic_event.event.msg.sender_pid = thread_getpid();

    evtimer_add_msg(&evtimer, &periodic_event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.ptr == &periodic_event) {
            periodic_callback(NULL);
        }
    }

    return 0;
}
