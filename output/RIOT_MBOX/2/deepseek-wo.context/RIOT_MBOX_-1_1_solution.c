
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE     (8)
#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(5);  // Send a message every 5 seconds
        msg_send(&msg, mbox_pid);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        SENDER_PRIO,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    return 0;
}
