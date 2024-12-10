
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
#define MBOX_QUEUE_SIZE     (8)
#define SENDER_THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static msg_t mbox_queue[MBOX_QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(1);  // Send a message every second
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, MBOX_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        NULL,
        SENDER_THREAD_STACK_SIZE,
        SENDER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        mbox_put(&mbox, &msg);
    }

    return 0;
}
