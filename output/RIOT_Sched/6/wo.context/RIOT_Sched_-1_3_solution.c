#include <thread_arch.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    printf("Thread running with arg: %d\n", *(int *)arg);
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;
    int arg1 = 1, arg2 = 2;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                CREATE_STACKTEST, thread_handler, &arg1, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                CREATE_STACKTEST, thread_handler, &arg2, "thread2");

    // Change the priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // Do nothing, just loop indefinitely
    }
}
