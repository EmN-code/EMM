#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define STACK_SIZE_THREAD1  (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD2  (THREAD_STACKSIZE_DEFAULT)

static char thread_stack1[STACK_SIZE_THREAD1];
static char thread_stack2[STACK_SIZE_THREAD2];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_func1(void *arg) {
    // Thread 1 Functionality
    printf("Thread 1 started with priority: %d\n", thread_get_priority(thread1_pid));

    // Simulate some processing
    for (int i = 0; i < 5; i++) {
        printf("Thread 1 is working\n");
        thread_yield();
    }

    return NULL;
}

void *thread_func2(void *arg) {
    // Thread 2 Functionality
    printf("Thread 2 started with priority: %d\n", thread_get_priority(thread2_pid));

    // Simulate some processing
    for (int i = 0; i < 5; i++) {
        printf("Thread 2 is working\n");
        thread_yield();
    }

    return NULL;
}

int main(void) {
    printf("Starting the example of changing thread priorities\n");

    // Create Thread 1
    thread1_pid = thread_create(thread_stack1, STACK_SIZE_THREAD1, THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread 1");

    // Create Thread 2
    thread2_pid = thread_create(thread_stack2, STACK_SIZE_THREAD2, THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread 2");

    // Change priority of Thread 1 to make it lower priority than Thread 2
    printf("Changing priority of Thread 1\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Wait for threads to finish
    thread_join(thread1_pid);
    thread_join(thread2_pid);

    printf("Threads have finished executing\n");
    return 0;
}