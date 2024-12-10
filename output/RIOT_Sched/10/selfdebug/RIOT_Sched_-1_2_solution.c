#include "thread.h"
#include "sched.h"
#include <stdio.h>

#define STACKSIZE_THREAD1   THREAD_STACKSIZE_MAIN
#define STACKSIZE_THREAD2   THREAD_STACKSIZE_MAIN
#define PRIORITY_THREAD1    (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD2    (THREAD_PRIORITY_MAIN - 2)

char thread_stack1[STACKSIZE_THREAD1];
char thread_stack2[STACKSIZE_THREAD2];

static void *thread_handler1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running with priority %d\n", thread_getpid());
        sched_yield(); // yield to other threads
    }
    return NULL;
}

static void *thread_handler2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running with priority %d\n", thread_getpid());
        sched_yield(); // yield to other threads
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread1, pid_thread2;

    // create two threads
    pid_thread1 = thread_create(thread_stack1, STACKSIZE_THREAD1, PRIORITY_THREAD1, 0,
                                thread_handler1, NULL, "thread1");
    pid_thread2 = thread_create(thread_stack2, STACKSIZE_THREAD2, PRIORITY_THREAD2, 0,
                                thread_handler2, NULL, "thread2");

    if (pid_thread1 > KERNEL_PID_UNDEF) {
        printf("Thread 1 started with PID %d\n", pid_thread1);
    }

    if (pid_thread2 > KERNEL_PID_UNDEF) {
        printf("Thread 2 started with PID %d\n", pid_thread2);
    }

    // Change priority of thread 1
    sched_change_priority(thread_getpid(), THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of Thread 1\n");

    while (1) {
        sched_yield(); // main thread can yield to other threads
    }

    return 0;
}