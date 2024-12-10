#include "thread_arch.h"
#include "thread.h"
#include "thread_config.h"
#include "sched.h"
#include "msg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "architecture.h"
#include "event/thread.h"

#define STACK_SIZE 1024

void *thread_func1(void *arg) {
    while (1) {
        // Thread function logic for thread 1
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        // Thread function logic for thread 2
    }
    return NULL;
}

int main(void) {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func2, NULL, "thread2");

    if (pid1 >= 0 && pid2 >= 0) {
        // Successfully created threads, now change the priority
        sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
        sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);
    }
    else {
        // Handle thread creation failure
    }

    return 0;
}