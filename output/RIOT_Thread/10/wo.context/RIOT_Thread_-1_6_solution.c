#include "thread.h"
#include <stdio.h>

#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t high_priority_thread_pid;
kernel_pid_t low_priority_thread_pid;

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("High priority thread received message: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Low priority thread received message: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    char hp_thread_stack[THREAD_STACKSIZE_MAIN];
    char lp_thread_stack[THREAD_STACKSIZE_MAIN];

    high_priority_thread_pid = thread_create(hp_thread_stack, sizeof(hp_thread_stack),
                                              HIGH_PRIORITY, THREAD_CREATE_STACKTEST,
                                              high_priority_thread, NULL, "hp_thread");

    low_priority_thread_pid = thread_create(lp_thread_stack, sizeof(lp_thread_stack),
                                            LOW_PRIORITY, THREAD_CREATE_STACKTEST,
                                            low_priority_thread, NULL, "lp_thread");

    msg_t msg = {.content.value = 42};
    msg_send(&msg, high_priority_thread_pid);
    msg_send(&msg, low_priority_thread_pid);

    return 0;
}