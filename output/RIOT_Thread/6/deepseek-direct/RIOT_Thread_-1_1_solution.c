
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

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
    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high),
                             THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                             thread_high, NULL, "thread_high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low),
                            THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                            thread_low, NULL, "thread_low");

    msg_t msg;

    // Send a message to the high priority thread
    msg.content.ptr = "Message to high priority thread";
    msg_send(&msg, pid_high);

    // Send a message to the low priority thread
    msg.content.ptr = "Message to low priority thread";
    msg_send(&msg, pid_low);

    // Sleep for a while to allow threads to process messages
    xtimer_sleep(1);

    return 0;
}
