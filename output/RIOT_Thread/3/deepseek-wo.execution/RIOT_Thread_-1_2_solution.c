
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t high_pid, low_pid;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg.content.value = 1;
    msg_send(&msg, high_pid);

    msg.content.value = 2;
    msg_send(&msg, low_pid);

    return 0;
}
