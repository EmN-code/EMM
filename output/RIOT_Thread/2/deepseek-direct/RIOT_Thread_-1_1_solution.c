
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    printf("High priority thread started\n");

    // Send a message to the low priority thread
    msg_t msg_to_send;
    msg_to_send.content.value = 42;
    msg_send(&msg_to_send, pid_low);

    // Wait for a reply from the low priority thread
    msg_receive(&msg);
    printf("High priority thread received reply: %d\n", msg.content.value);

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    printf("Low priority thread started\n");

    // Wait for a message from the high priority thread
    msg_receive(&msg);
    printf("Low priority thread received message: %d\n", msg.content.value);

    // Send a reply back to the high priority thread
    msg_t msg_reply;
    msg_reply.content.value = 1337;
    msg_send(&msg_reply, pid_high);

    return NULL;
}

int main(void)
{
    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    printf("Main thread started\n");

    // Main thread does not need to do anything else
    return 0;
}
