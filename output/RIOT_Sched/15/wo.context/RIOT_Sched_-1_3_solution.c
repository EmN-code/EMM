#include "xtimer.h"
#include "thread.h"
#include "stdio.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char thread1_stack[THREAD1_STACKSIZE];
static char thread2_stack[THREAD2_STACKSIZE];

void *thread1_handler(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        xtimer_sleep(2);
        printf("Thread 1 doing work\n");
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        xtimer_sleep(3);
        printf("Thread 2 doing work\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    pid1 = thread_create(thread1_stack, THREAD1_STACKSIZE, THREAD1_PRIORITY,
                         THREAD_CREATE_WOUT_YIELD, thread1_handler, NULL, "thread1");
    pid2 = thread_create(thread2_stack, THREAD2_STACKSIZE, THREAD2_PRIORITY,
                         THREAD_CREATE_WOUT_YIELD, thread2_handler, NULL, "thread2");
    
    /* Simulate some delay before changing priority */
    xtimer_sleep(5);
    thread_t *thread1 = thread_get(pid1);
    thread_t *thread2 = thread_get(pid2);
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 4);
    
    while (1) {
        xtimer_sleep(1);
        printf("Main thread running\n");
    }
    return 0;
}
