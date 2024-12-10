
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t high_priority_thread_pid;
kernel_pid_t low_priority_thread_pid;

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
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
    char *high_priority_stack = malloc(STACKSIZE);
    char *low_priority_stack = malloc(STACKSIZE);

    if (high_priority_stack == NULL || low_priority_stack == NULL) {
        puts("Failed to allocate stack memory");
        return 1;
    }

    high_priority_thread_pid = thread_create(high_priority_stack, STACKSIZE, PRIORITY_HIGH,
                                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority");

    low_priority_thread_pid = thread_create(low_priority_stack, STACKSIZE, PRIORITY_LOW,
                                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");

    msg.content.ptr = "Message for high priority thread";
    msg_send(&msg, high_priority_thread_pid);

    msg.content.ptr = "Message for low priority thread";
    msg_send(&msg, low_priority_thread_pid);

    return 0;
}
