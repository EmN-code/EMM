
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        xtimer_usleep(INTERVAL_MS);
        msg.content.value = 1;
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
