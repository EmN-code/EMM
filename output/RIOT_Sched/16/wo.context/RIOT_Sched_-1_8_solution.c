#include "thread.h"
#include "stdio.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread one running\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread two running\n");
    return NULL;
}

int main(void) {
    printf("Starting threads\n");
    
    thread_one_pid = thread_create(stack_one, THREAD_STACKSIZE_MAIN,
                                   THREAD_PRIORITY_MAIN, 0, thread_one, NULL, "ThreadOne");

    thread_two_pid = thread_create(stack_two, THREAD_STACKSIZE_MAIN,
                                   THREAD_PRIORITY_MAIN, 0, thread_two, NULL, "ThreadTwo");
    
    // Change priority
    if (thread_one_pid > 0) {
        sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 1);
    }

    if (thread_two_pid > 0) {
        sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 2);
    }

    return 0;
}