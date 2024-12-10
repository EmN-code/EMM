
#include <stdio.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 1024

void thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(sched_active_pid()));
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
}

void thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(sched_active_pid()));
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    printf("Main thread started\n");

    // Change priority of thread 1
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1 to %d\n", THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread 2
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);
    printf("Changed priority of thread 2 to %d\n", THREAD_PRIORITY_MAIN - 4);

    while (1) {
        printf("Main thread running\n");
        thread_yield();
    }

    return 0;
}
