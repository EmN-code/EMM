
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_high_priority(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *thread_low_priority(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          thread_high_priority, NULL, "high");

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         thread_low_priority, NULL, "low");

    // Send messages to the threads
    msg_t msg;
    msg.content.value = 123;

    msg_send(&msg, pid_high);
    msg_send(&msg, pid_low);

    return 0;
}
