
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <thread.h>
#include <sched.h>

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created with PIDs: %d and %d\n", pid_1, pid_2);

    // Change priority of thread 1 to a higher priority
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 1);
    printf("Changed priority of thread 1 to higher\n");

    // Change priority of thread 2 to a lower priority
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to lower\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
