
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "msg.h"

#define STACK_SIZE 512
#define NUM_MESSAGES 16

char stack_low_prio[STACK_SIZE];
char stack_high_prio[STACK_SIZE];

msg_t msg_queue[NUM_MESSAGES];

void *low_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *high_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t low_prio_pid, high_prio_pid;
    msg_t msg;

    msg_init_queue(msg_queue, NUM_MESSAGES);

    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 low_prio_thread, NULL, "low_prio");

    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                  high_prio_thread, NULL, "high_prio");

    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, low_prio_pid);
        msg_send(&msg, high_prio_pid);
        sleep(1);
    }

    return 0;
}
