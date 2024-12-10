
#include <stdlib.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <thread.h>
#include <msg.h>

#define STACKSIZE 2048
#define NUM_MESSAGES 8

char stack_low[STACKSIZE];
char stack_high[STACKSIZE];

void low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
}

void high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    kernel_pid_t low_pid = thread_create(stack_low, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, low_priority_thread, NULL, "low");
    kernel_pid_t high_pid = thread_create(stack_high, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, high_priority_thread, NULL, "high");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 1234;

    msg_send(&msg, low_pid);
    msg_send(&msg, high_pid);

    return 0;
}
