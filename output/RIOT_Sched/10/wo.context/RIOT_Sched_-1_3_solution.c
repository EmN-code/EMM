#include <thread.h>
#include <sched.h>
#include <stdio.h>

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_one_function(void *arg) {
    printf("Thread one running\n");
    thread_yield();
    return NULL;
}

void *thread_two_function(void *arg) {
    printf("Thread two running\n");
    thread_yield();
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;

    thread_one_pid = thread_create(stack_one, sizeof(stack_one),
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST,
                                   thread_one_function, NULL, "thread_one");

    thread_two_pid = thread_create(stack_two, sizeof(stack_two),
                                   THREAD_PRIORITY_MAIN - 2,
                                   THREAD_CREATE_STACKTEST,
                                   thread_two_function, NULL, "thread_two");

    if (thread_one_pid == KERNEL_PID_UNDEF || thread_two_pid == KERNEL_PID_UNDEF) {
        printf("Error creating threads\n");
        return -1;
    }

    // Change priority of threads at runtime
    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}