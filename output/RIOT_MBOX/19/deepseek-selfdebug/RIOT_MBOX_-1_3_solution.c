
#include "thread.h"
#include "mbox.h"
#include "evtimer.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (16)
#define EVENT_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void event_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);
    evtimer_add_msg(&evtimer, &event_callback, NULL, EVENT_INTERVAL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, -1);
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
