
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 8
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS / US_PER_SEC);
        msg_send(&msg, mbox_pid(&mbox));
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
