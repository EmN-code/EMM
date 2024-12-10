#include "thread_arch.h"
#include "thread_config.h"
#include "stdio.h"

// Thread stack sizes and priorities
#define THREAD_1_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define THREAD_2_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

static char stack_thread1[THREAD_STACKSIZE_MAIN];
static char stack_thread2[THREAD_STACKSIZE_MAIN];

// Thread function for thread 1
void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 running with priority %d\n", THREAD_1_PRIO);
    while (1) {
        // Simulating some work
        for (volatile int i = 0; i < 1000000; ++i);
        printf("Thread 1: Working\n");
    }
    return NULL;
}

// Thread function for thread 2
void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 running with priority %d\n", THREAD_2_PRIO);
    while (1) {
        // Simulating some work
        for (volatile int i = 0; i < 1000000; ++i);
        printf("Thread 2: Working\n");
    }
    return NULL;
}

int main(void) {
    // Thread IDs
    kernel_pid_t thread1_pid, thread2_pid;
    
    // Create threads
    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1), THREAD_1_PRIO, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2), THREAD_2_PRIO, 0, thread2_func, NULL, "thread2");

    printf("Threads started\n");

    // Change priorities at runtime
    int new_priority_thread1 = THREAD_PRIORITY_MAIN - 3;
    int new_priority_thread2 = THREAD_PRIORITY_MAIN - 4;
    sched_change_priority(thread1_pid, new_priority_thread1);
    sched_change_priority(thread2_pid, new_priority_thread2);

    printf("Priorities changed: thread1 -> %d, thread2 -> %d\n", new_priority_thread1, new_priority_thread2);
    
    // Prevent exit
    while (1) {
        // Main thread can be used for additional tasks or just yield
        thread_yield();
    }

    return 0;
}