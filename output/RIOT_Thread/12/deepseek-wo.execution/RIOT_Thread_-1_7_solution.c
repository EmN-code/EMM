
#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

static char stack_low_prio[STACK_SIZE];
static char stack_high_prio[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *low_prio_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *high_prio_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                              low_prio_thread, NULL, "low_prio");

    kernel_pid_t high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                               THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                               high_prio_thread, NULL, "high_prio");

    msg_t msg;
    msg.content.value = 123;

    // Send message to low priority thread
    msg_send(&msg, low_prio_pid);

    // Send message to high priority thread
    msg_send(&msg, high_prio_pid);

    return 0;
}
