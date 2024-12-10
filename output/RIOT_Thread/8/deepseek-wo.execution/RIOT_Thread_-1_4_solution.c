
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define STACK_SIZE 1024
#define NUM_MESSAGES 8

static char stack_low_prio[STACK_SIZE];
static char stack_high_prio[STACK_SIZE];

static msg_t msg_array[NUM_MESSAGES];

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_prio_pid, high_prio_pid;
    msg_t msg;

    // Initialize the message queue
    msg_init_queue(msg_array, NUM_MESSAGES);

    // Create low priority thread
    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1, 0, low_priority_thread,
                                 NULL, "low_prio");

    // Create high priority thread
    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN - 2, 0, high_priority_thread,
                                  NULL, "high_prio");

    // Send messages to threads
    msg.content.value = 1;
    msg_send(&msg, low_prio_pid);

    msg.content.value = 2;
    msg_send(&msg, high_prio_pid);

    return 0;
}
