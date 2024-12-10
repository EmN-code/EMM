
#include "mbox.h"
#include "evtimer.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define MSG_QUEUE_SIZE 8
#define EVENT_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);

    evtimer_event_t event;
    evtimer_event_init_msg(&event, xtimer_ticks64(EVENT_INTERVAL), event_handler, NULL);

    evtimer_add(&evtimer, &event);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
