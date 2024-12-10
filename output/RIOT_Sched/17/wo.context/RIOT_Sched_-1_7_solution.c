#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread One Running\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread Two Running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_one, NULL, "Thread One");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, thread_two, NULL, "Thread Two");

    /* Changing the priority of the threads at runtime */
    thread_t *thr_one = thread_get(pid_one);
    thread_t *thr_two = thread_get(pid_two);

    sched_change_priority(thr_one, THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thr_two, THREAD_PRIORITY_MAIN + 3);

    return 0;
}