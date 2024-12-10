
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;
static msg_t msg_queue[8];
static mbox_t mbox;

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                               event_thread, NULL, "event_thread");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(5);
        mbox_put(&mbox, &msg);
        msg.content.value++;
    }

    return 0;
}
