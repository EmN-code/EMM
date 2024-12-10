#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define STACK_SIZE_THREAD1 (THREAD_STACKSIZE_MAIN)
#define STACK_SIZE_THREAD2 (THREAD_STACKSIZE_MAIN)
#define PRIORITY_THREAD1 (THREAD_PRIORITY_MAIN + 1)
#define PRIORITY_THREAD2 (THREAD_PRIORITY_MAIN + 2)

char stack1[STACK_SIZE_THREAD1];
char stack2[STACK_SIZE_THREAD2];

void *thread_func1(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    while (1) {
        printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    while (1) {
        printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    puts("Starting threads on ESP32 with RIOT...");

    thread1_pid = thread_create(stack1, STACK_SIZE_THREAD1, PRIORITY_THREAD1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread 1");
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread 1");
        return 1;
    }

    thread2_pid = thread_create(stack2, STACK_SIZE_THREAD2, PRIORITY_THREAD2, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread 2");
    if (thread2_pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread 2");
        return 1;
    }

    xtimer_sleep(5);
    printf("Changing priority of thread 1\n");
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN + 3);

    xtimer_sleep(5);
    printf("Changing priority of thread 2\n");
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN + 4);

    return 0;
}