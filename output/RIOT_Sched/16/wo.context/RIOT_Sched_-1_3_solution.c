#include "thread_config.h"
#include "kernel_defines.h"
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread %d is running\n", thread_getpid());
    return NULL;
}

int main(void) {
    puts("RIOT project starting\n");
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY, 0, thread_func, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY, 0, thread_func, NULL, "thread2");
    
    // Example of changing thread priorities
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}