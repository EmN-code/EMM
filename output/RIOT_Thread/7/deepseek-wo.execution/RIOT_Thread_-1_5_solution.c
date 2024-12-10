
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <thread.h>
#include <msg.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue_high[MSG_QUEUE_SIZE];
static msg_t msg_queue_low[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;
    msg_t msg;

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, pid_high);
        msg_send(&msg, pid_low);
    }

    return 0;
}
