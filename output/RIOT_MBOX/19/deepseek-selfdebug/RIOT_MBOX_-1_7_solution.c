
#include "msg.h"
#include "thread.h"
#include "evtimer.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init(&evtimer);

    evtimer_event.event.offset = 1000000;  // 1 second
    evtimer_event.event.callback = event_callback;
    evtimer_event.event.arg = NULL;
    evtimer_event.mbox = &_main_msg_queue;

    evtimer_add_msg(&evtimer, &evtimer_event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
