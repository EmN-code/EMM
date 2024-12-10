
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 8
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
