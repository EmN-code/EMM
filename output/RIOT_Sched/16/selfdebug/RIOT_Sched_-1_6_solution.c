#include <thread_arch.h>
#include <thread.h>
#include <sched.h>
#include <stdio.h>

#define STACKSIZE THREAD_STACKSIZE_DEFAULT

void *thread_func(void *arg) {
    (void)arg; // To prevent unused variable warning
    while (1) {
        printf("Running thread with argument %d\n", (int)arg);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];
    kernel_pid_t pid1, pid2;
    
    pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_func, (void *)1, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, (void *)2, "thread2");

    // Change priority at runtime
    xtimer_sleep(5);
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread1\n");

    return 0;
}