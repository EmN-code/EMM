#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define THREAD_CREATE_FLAGS (THREAD_CREATE_STACKTEST)

char stack_thread1[STACKSIZE];
char stack_thread2[STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 executing\n");
    while (1) {
        // Thread execution code
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 executing\n");
    while (1) {
        // Thread execution code
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack_thread1, STACKSIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_FLAGS, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack_thread2, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_FLAGS, thread2_func, NULL, "thread2");

    // Change priority of thread1 to be lower than thread2
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // Main execution code
    }
    return 0;
}