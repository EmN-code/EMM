
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "msg.h"

#define STACK_SIZE 512
#define NUM_MESSAGES 16

char stack_high[STACK_SIZE];
char stack_low[STACK_SIZE];

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1, 0, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, pid_high);
        msg_send(&msg, pid_low);
        xtimer_sleep(1);
    }

    return 0;
}
