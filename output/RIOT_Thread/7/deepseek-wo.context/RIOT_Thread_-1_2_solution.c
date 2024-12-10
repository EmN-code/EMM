
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[STACKSIZE];
char stack_low[STACKSIZE];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1234567890; // Example value

    printf("Sending message from high priority thread\n");
    msg_send(&msg, pid_low);

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_receive(&msg);
    printf("Received message in low priority thread with value: %lu\n", (unsigned long)msg.content.value);

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high_prio");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low_prio");

    return 0;
}
