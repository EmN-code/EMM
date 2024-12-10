
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_t msg;
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    high_priority_pid = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                                      THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority");

    low_priority_pid = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                                     THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");

    msg.type = 0;
    msg.content.value = 12345;

    msg_send(&msg, high_priority_pid);
    msg_send(&msg, low_priority_pid);

    return 0;
}
