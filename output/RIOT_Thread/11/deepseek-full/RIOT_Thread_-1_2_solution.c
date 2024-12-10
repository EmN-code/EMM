
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
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
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                          high_priority_thread, NULL, "high_priority");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                         low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.content.value = 123;

    while (1) {
        msg.sender_pid = high_pid;
        msg_send(&msg, low_pid);
        xtimer_sleep(1);

        msg.sender_pid = low_pid;
        msg_send(&msg, high_pid);
        xtimer_sleep(1);
    }

    return 0;
}
