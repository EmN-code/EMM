
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
    msg_t msg_queue[10];

    msg_init_queue(msg_queue, 10);

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
    msg.content.ptr = "Hello from high priority thread!";

    while (1) {
        msg_send(&msg, low_prio_pid);
        xtimer_sleep(1); // Sleep for 1 second
    }

    return NULL;
}

int main(void)
{
    // Create low priority thread
    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST,
                                 low_prio_thread, NULL, "low_prio");

    // Create high priority thread
    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN + 1,
                                  THREAD_CREATE_STACKTEST,
                                  high_prio_thread, NULL, "high_prio");

    return 0;
}
