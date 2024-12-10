
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        msg_send(&msg, mbox_pid);
        xtimer_usleep(INTERVAL_MS);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    return 0;
}
