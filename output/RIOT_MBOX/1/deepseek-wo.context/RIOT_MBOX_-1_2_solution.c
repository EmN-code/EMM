
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 8
#define INTERVAL_MS 1000000

kernel_pid_t mbox_pid;

void *mbox_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_t msg;
    mbox_t mbox;

    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    mbox_t mbox;

    mbox_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        mbox_thread,
        NULL,
        "mbox_thread"
    );

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg.content.ptr = "Hello from main thread";
        msg.sender_pid = thread_getpid();
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
