
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define STACKSIZE 1024

char stack_low_prio[STACKSIZE];
char stack_high_prio[STACKSIZE];

kernel_pid_t low_prio_pid;
kernel_pid_t high_prio_pid;

void *low_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    // Send a message to the low priority thread
    msg.content.ptr = "Hello from high priority thread!";
    msg_send(&msg, low_prio_pid);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    // Create low priority thread
    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 low_prio_thread, NULL, "low_prio");

    // Create high priority thread
    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                                  high_prio_thread, NULL, "high_prio");

    // Send a message to the high priority thread
    msg_t msg;
    msg.content.ptr = "Hello from main!";
    msg_send(&msg, high_prio_pid);

    return 0;
}
