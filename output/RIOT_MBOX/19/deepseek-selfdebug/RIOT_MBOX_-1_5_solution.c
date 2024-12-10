
#include "msg.h"
#include "thread.h"
#include "evtimer.h"
#include "mbox.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_msgs[8];

static evtimer_t evtimer;
static evtimer_msg_t events[2];

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, -1);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_msgs, 8);

    evtimer_init(&evtimer);

    for (unsigned i = 0; i < 2; i++) {
        events[i].event.offset = (i + 1) * 1000000U;
        events[i].event.msg.content.value = i + 1;
        evtimer_add_msg(&evtimer, &events[i]);
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    return 0;
}
