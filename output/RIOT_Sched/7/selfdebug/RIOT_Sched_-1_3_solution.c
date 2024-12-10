#include <thread.h>
#include <stdio.h>

void *thread_one(void *arg) {
    printf("Thread One is running\n");
    // Simulate some work being done by this thread
    for (int i = 0; i < 5; i++) {
        printf("Thread One Loop: %d\n", i);
    }
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread Two is running\n");
    // Simulate some work being done by this thread
    for (int i = 0; i < 5; i++) {
        printf("Thread Two Loop: %d\n", i);
    }
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE_MAIN];
    char stack_two[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");
    
    printf("Threads created successfully\n");

    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);

    printf("Priority changed successfully\n");

    thread_yield();
    
    return 0;
}