#include <thread_arch.h>
#include <stdio.h>
#include <kernel/thread.h>

#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char thread1_stack[THREAD_STACKSIZE];
char thread2_stack[THREAD_STACKSIZE];

void *thread1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");

    // Simulate work
    for (int i = 0; i < 10; i++) {
        printf("Thread 1: iteration %d\n", i);
    }

    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");

    // Simulate work
    for (int i = 0; i < 10; i++) {
        printf("Thread 2: iteration %d\n", i);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    printf("Starting threads\n");

    // Create and start threads
    thread1_pid = thread_create(thread1_stack, THREAD_STACKSIZE, THREAD1_PRIORITY, 0,
                                thread1, NULL, "thread1");
    if (thread1_pid < 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    thread2_pid = thread_create(thread2_stack, THREAD_STACKSIZE, THREAD2_PRIORITY, 0,
                                thread2, NULL, "thread2");
    if (thread2_pid < 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Change priority of thread 2
    printf("Changing priority of thread 2\n");
    if (sched_set_status(thread2_pid, THREAD_PRIORITY_MIN) != 0) {
        printf("Failed to change priority for thread 2\n");
        return 1;
    }

    // Keep the main alive to let threads execute
    while (1) {}

    return 0;
}