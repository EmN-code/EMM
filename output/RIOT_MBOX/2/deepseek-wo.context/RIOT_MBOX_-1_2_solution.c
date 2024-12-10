
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS / US_PER_SEC);
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_t *sender_thread = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    msg_t received_msg;
    while (1) {
        mbox_get(&mbox, &received_msg);
        printf("Received message with value: %d\n", received_msg.content.value);
    }

    return 0;
}
