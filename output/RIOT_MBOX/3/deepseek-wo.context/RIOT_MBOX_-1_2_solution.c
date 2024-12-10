
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"
#include "evtimer.h"

#define QUEUE_SIZE 16
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init(&evtimer);

    evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)event_handler, thread_getpid());

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received event\n");
    }

    return 0;
}
