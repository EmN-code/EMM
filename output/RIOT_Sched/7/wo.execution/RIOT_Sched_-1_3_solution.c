#include "thread_config.h"
#include "sched.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_func1(void *arg) {
    while (1) {
        // Thread 1 functionality
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        // Thread 2 functionality
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");
    
    // Change priority of thread1 and thread2
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}