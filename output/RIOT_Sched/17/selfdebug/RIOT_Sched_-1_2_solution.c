#include <stdio.h>
#include <thread_config.h>
#include <xtimer.h>
#include <sched.h>
#include <thread.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_FLAGS THREAD_CREATE_STACKTEST

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_one(void *arg) {
    while(1) {
        printf("Running thread one\n");
        thread_yield();
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while(1) {
        printf("Running thread two\n");
        thread_yield();
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY, THREAD_FLAGS, thread_one, NULL, "thread_one");
    if (pid_one < 0) {
        printf("Error creating thread one\n");
        return 1;
    }

    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY, THREAD_FLAGS, thread_two, NULL, "thread_two");
    if (pid_two < 0) {
        printf("Error creating thread two\n");
        return 1;
    }

    printf("Threads running, changing priority\n");

    xtimer_sleep(2);
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY + 1);

    while(1) {}
    return 0;
}
