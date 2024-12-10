#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN 1024
#define PRIORITY_THREAD_1     (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_2     (THREAD_PRIORITY_MAIN - 2)

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running with priority: %d\n", PRIORITY_THREAD_1);
    while (1) {
        // Simulate thread task
        xtimer_sleep(3);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running with priority: %d\n", PRIORITY_THREAD_2);
    while (1) {
        // Simulate thread task
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_THREAD_1, THREAD_CREATE_STACKTEST,
                         thread_func1, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_THREAD_2, THREAD_CREATE_STACKTEST,
                         thread_func2, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    printf("Threads created successfully\n");

    // Change priority of thread 1 at runtime
    thread_set_priority(pid1, PRIORITY_THREAD_1 - 1);
    printf("Changed priority of thread 1 to: %d\n", PRIORITY_THREAD_1 - 1);

    // Let main thread idle
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}