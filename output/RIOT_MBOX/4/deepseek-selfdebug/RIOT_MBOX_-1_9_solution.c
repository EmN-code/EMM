
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(2);  // Send a message every 2 seconds
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, -1);  // Blocking call to get a message
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_SLEEPING,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_SLEEPING,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
