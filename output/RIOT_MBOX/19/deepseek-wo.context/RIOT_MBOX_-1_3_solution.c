
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define SEND_INTERVAL (1000000U)  // 1 second

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_usleep(SEND_INTERVAL);
        msg_send(&msg, thread_getpid());
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    kernel_pid_t receiver_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
