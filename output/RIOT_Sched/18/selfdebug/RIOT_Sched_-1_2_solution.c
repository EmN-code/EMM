#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 7

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread %u is running\n", (unsigned) thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                          THREAD_CREATE_STACKTEST, thread_function, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                          THREAD_CREATE_STACKTEST, thread_function, NULL, "thread2");

    while (1) {
        xtimer_sleep(5);
        printf("Changing priorities\n");
        sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
        sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 1);
    }

    return 0;
}