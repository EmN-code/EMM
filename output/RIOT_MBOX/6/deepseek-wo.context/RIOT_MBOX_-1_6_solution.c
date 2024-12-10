
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS / US_PER_SEC);
        mbox_put(&mbox, &msg);
    }
}

int main(void)
{
    msg_t msg;
    kernel_pid_t sender_pid;

    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    sender_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
