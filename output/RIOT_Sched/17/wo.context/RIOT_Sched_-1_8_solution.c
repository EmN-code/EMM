#include <stdio.h>
#include <stdint.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE    (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_ONE  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_TWO  (THREAD_PRIORITY_MAIN - 2)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_ONE, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread_one");
    kernel_pid_t thread_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_TWO, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread_two");

    printf("Threads created\n");

    // Simulate changing thread priority at runtime
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_TWO);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_ONE);

    return 0;
}