#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "sched.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_DEFAULT 512

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority: %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t t1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                                    thread_func, NULL, "thread 1");
    kernel_pid_t t2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
                                    thread_func, NULL, "thread 2");

    printf("Threads created, running at different priorities\n");

    // Simulating runtime change of priority
    xtimer_sleep(3);
    sched_change_priority(thread_get(t1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(t2), THREAD_PRIORITY_MAIN - 1);

    printf("Changed priorities, now t1 has lower and t2 has higher priority\n");

    // Let the program run indefinitely
    while (1) {}

    return 0;
}