#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 3)

// Stack for Thread 1
char stack1[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread1_pid;

// Stack for Thread 2
char stack2[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    printf("Thread 1 is running with high priority!\n");
    return NULL;
}

void *thread2_func(void *arg) {
    printf("Thread 2 is running with low priority!\n");
    return NULL;
}

int main(void) {
    printf("Starting RIOT OS application on ESP32\n");
    
    // Create thread 1 with high priority
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create thread 2 with low priority
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of threads at runtime
    thread_t *thread1 = thread_get(thread1_pid);
    thread_t *thread2 = thread_get(thread2_pid);

    printf("Changing priority of Thread 1 to low priority\n");
    if (thread1) {
        sched_change_priority(thread1, THREAD_PRIORITY_MAIN + 1);
    }

    printf("Changing priority of Thread 2 to high priority\n");
    if (thread2) {
        sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 1);
    }

    while(1) {
        // Main thread can do other tasks or sleep
    }

    return 0;
}