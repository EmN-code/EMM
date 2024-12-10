#include "thread_config.h"

#include "thread.h"
#include "sched.h"
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT  (1024)
#define THREAD_PRIORITY_MAIN      (0)
#define THREAD_CREATE_STACKTEST   (1 << 0)

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_one(void *arg) {
    printf("Thread one is running\n");
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread two is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one;
    kernel_pid_t pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    if (pid_one >= 0) {
        printf("Thread one created successfully\n");
    } else {
        printf("Error creating thread one\n");
    }

    if (pid_two >= 0) {
        printf("Thread two created successfully\n");
    } else {
        printf("Error creating thread two\n");
    }

    // Change priority after threads are created
    sched_change_priority(pid_one, THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(pid_two, THREAD_PRIORITY_MAIN + 3);

    return 0;
}