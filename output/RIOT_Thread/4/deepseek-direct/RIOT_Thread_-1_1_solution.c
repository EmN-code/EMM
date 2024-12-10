
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack_high[THREAD_STACKSIZE_DEFAULT];
static char stack_low[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    // Send a message to the high priority thread
    msg.content.ptr = "Message to high priority thread";
    msg_send(&msg, pid_high);

    // Send a message to the low priority thread
    msg.content.ptr = "Message to low priority thread";
    msg_send(&msg, pid_low);

    return 0;
}
