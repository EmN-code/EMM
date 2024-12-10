
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "msg.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);

    event.event.offset = 1 * US_PER_SEC;
    event.event.next = NULL;
    event.msg.type = EVTIMER_MSG_TYPE;
    event.msg.content.ptr = NULL;

    evtimer_add_msg(&evtimer, &event);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, periodic_callback, NULL, "periodic_callback");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            periodic_callback(NULL);
            evtimer_add_msg(&evtimer, &event);
        }
    }

    return 0;
}
