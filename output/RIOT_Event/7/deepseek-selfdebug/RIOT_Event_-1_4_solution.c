
#include "xtimer.h"
#include "event.h"
#include "evtimer.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_msg_event_t event;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    event.event.handler = (event_handler_t)event_periodic_callback;
    event.msg.sender_pid = thread_getpid();

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        xtimer_sleep(1);
    }

    return 0;
}
