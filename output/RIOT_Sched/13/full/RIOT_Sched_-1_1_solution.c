#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char stack_thread1[THREAD_STACKSIZE_MAIN];
char stack_thread2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread running with priority: %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_1_pid;
    kernel_pid_t thread_2_pid;

    puts("Starting two threads and changing priorities.");

    thread_1_pid = thread_create(stack_thread1, sizeof(stack_thread1), THREAD_PRIORITY_MAIN - 1,
                                 CREATE_STACKTEST, thread_handler, NULL, "thread 1");

    thread_2_pid = thread_create(stack_thread2, sizeof(stack_thread2), THREAD_PRIORITY_MAIN - 2,
                                 CREATE_STACKTEST, thread_handler, NULL, "thread 2");

    /* Change thread 1 priority */
    puts("Changing priority of thread 1.");
    sched_change_priority(thread_1_pid, THREAD_PRIORITY_MAIN - 3);

    /* Change thread 2 priority */
    puts("Changing priority of thread 2.");
    sched_change_priority(thread_2_pid, THREAD_PRIORITY_MAIN - 4);

    while (1) {
        thread_yield();
    }
    return 0;
}