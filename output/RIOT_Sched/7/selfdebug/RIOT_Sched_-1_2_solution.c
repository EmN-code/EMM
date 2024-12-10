#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_IDLE + 1)
#define THREAD_STACKSIZE_MAIN   THREAD_STACKSIZE_DEFAULT

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread started with PID: %d\n", thread_getpid());
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;
    printf("Main thread started\n");

    thread1 = thread_create(stack1, sizeof(stack1),
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            thread_func, NULL, "thread1");
    if (thread1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    thread2 = thread_create(stack2, sizeof(stack2),
                            THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST,
                            thread_func, NULL, "thread2");
    if (thread2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change priority of thread1 after creation
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1\n");

    // Yield the main thread to allow other threads to run
    thread_yield();

    return 0;
}