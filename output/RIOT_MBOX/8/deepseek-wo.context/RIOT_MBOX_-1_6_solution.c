
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 8
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(2);
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, SENDER_PRIO, 0, sender_thread, NULL, "sender");
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");

    return 0;
}
