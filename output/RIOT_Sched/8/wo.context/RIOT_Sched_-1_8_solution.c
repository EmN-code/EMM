#include <stdio.h>
#include <kernel.h>
#include <thread.h>

#define PRIO_HIGH   (THREAD_PRIORITY_MAIN - 1)
#define PRIO_LOW    (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE   (THREAD_STACKSIZE_MAIN)

static char thread_stack1[STACKSIZE];
static char thread_stack2[STACKSIZE];

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread started with PID: %d\n", thread_getpid());
    while(1) {
        /* Simulate some activity */
        printf("Thread running with priority: %u\n", (unsigned) thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    /* Create two threads */
    kernel_pid_t pid1 = thread_create(thread_stack1, STACKSIZE, PRIO_HIGH, 0, thread_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(thread_stack2, STACKSIZE, PRIO_LOW, 0, thread_func, NULL, "thread2");

    /* Allow threads some time to start and run */
    xtimer_sleep(5);
    
    /* Change priority of the threads at runtime */
    sched_change_priority(thread_get(pid1), PRIO_LOW);
    sched_change_priority(thread_get(pid2), PRIO_HIGH);

    printf("Priorities changed: thread1 now has low priority, thread2 has high priority\n");

    /* Keep main thread alive */
    while (1) {}
    
    return 0;
}