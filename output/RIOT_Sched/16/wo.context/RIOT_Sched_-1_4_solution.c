#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>

#define THREAD1_STACK_SIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD2_STACK_SIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD1_PRIORITY      (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY      (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[THREAD1_STACK_SIZE];
char thread2_stack[THREAD2_STACK_SIZE];

void *thread_func1(void *arg) {
    printf("Thread 1 is running\n");
    while (1) {
        /* Insert thread's main loop functionality */
    }
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running\n");
    while (1) {
        /* Insert thread's main loop functionality */
    }
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY, 0,
                                thread_func1, NULL, "thread1");
    printf("Thread 1 started with PID: %d\n", thread1_pid);

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY, 0,
                                thread_func2, NULL, "thread2");
    printf("Thread 2 started with PID: %d\n", thread2_pid);

    thread_t *thread1 = thread_get(thread1_pid);
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN);

    thread_t *thread2 = thread_get(thread2_pid);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN + 1);

    return 0;
}