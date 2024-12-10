
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <thread.h>
#include <msg.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

char stack_high[STACK_SIZE];
char stack_low[STACK_SIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");
    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
    }

    return 0;
}
