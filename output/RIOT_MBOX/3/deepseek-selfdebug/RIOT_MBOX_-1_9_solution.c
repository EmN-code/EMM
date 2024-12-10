
#include <stdio.h>
#include <thread.h>
#include <mbox.h>
#include <ztimer.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(NULL, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(NULL, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
