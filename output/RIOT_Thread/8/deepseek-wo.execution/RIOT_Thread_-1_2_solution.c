
#include "thread.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "event/thread.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    msg.content.value = 42;
    msg.type = 0;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t high_pid, low_pid;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}
