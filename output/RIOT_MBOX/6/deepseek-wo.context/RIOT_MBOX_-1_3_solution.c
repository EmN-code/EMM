
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE (8)

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, -1);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(thread_stack, sizeof(thread_stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     event_thread, NULL, "event_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create event thread\n");
        return 1;
    }

    uint32_t event_value = 0;

    while (1) {
        msg_t msg;
        msg.content.value = event_value++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return 0;
}
