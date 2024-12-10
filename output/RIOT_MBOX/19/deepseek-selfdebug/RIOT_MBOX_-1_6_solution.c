
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <evtimer.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_callback(void *arg)
{
    msg_t msg;
    msg.content.value = 0x1234;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);

    evtimer_msg_event_t event;
    event.event.offset = INTERVAL_MS;
    event.event.callback = event_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    evtimer_add_msg(&evtimer, &event);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
