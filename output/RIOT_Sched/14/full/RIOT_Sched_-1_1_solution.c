#include <thread_config.h>
#include <thread_arch.h>
#include <stdio.h>

void *thread_func_1(void *arg) {
    while (1) {
        printf("Thread 1 is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    while (1) {
        printf("Thread 2 is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_1, thread_2;
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    
    thread_1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                             CREATE_STACKTEST, thread_func_1, NULL, "thread_1");
    if (thread_1 < 0) {
        printf("Error: Thread 1 not created\n");
    }
    
    thread_2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                             CREATE_STACKTEST, thread_func_2, NULL, "thread_2");
    if (thread_2 < 0) {
        printf("Error: Thread 2 not created\n");
    }

    // Change the priority of thread 1 at runtime
    sched_change_priority(thread_1, THREAD_PRIORITY_MAIN - 3);

    // Keep main thread running so that child threads can execute
    while (1) {
        thread_yield();
    }

    return 0;
}