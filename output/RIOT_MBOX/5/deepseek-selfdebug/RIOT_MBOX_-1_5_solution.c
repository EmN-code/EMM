
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "event.h"
#include "event/callback.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[8];

static void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, sizeof(mbox_queue) / sizeof(msg_t));

    event_queue_t event_queue;
    event_thread_init(&event_queue, NULL, 0, 0);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, event_handler);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
