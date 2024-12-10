
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1;  // Dummy value for the message
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_receiver,
        NULL,
        "event_receiver"
    );

    return 0;
}
