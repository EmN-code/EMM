
#include "thread.h"
#include "msg.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdatomic.h"

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                         low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_high);
        } else {
            msg_send(&msg, pid_low);
        }
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
