
#include <stdio.h>
#include "thread.h"
#include "mbox.h"
#include "ztimer.h"
#include "msg.h"

#define MSG_QUEUE_SIZE (16)
#define INTERVAL_MS (1000)

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with content: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    kernel_pid_t pid;

    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    while (1) {
        msg.content.value = ztimer_now(ZTIMER_MSEC);
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
    }

    return 0;
}
