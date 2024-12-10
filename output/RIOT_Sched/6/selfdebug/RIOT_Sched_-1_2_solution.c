#include "kernel_defines.h"
#include "thread.h"

#define THREAD_ONE_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_TWO_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_ONE_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_TWO_PRIORITY     (THREAD_PRIORITY_MAIN - 2)

char thread_one_stack[THREAD_ONE_STACKSIZE];
char thread_two_stack[THREAD_TWO_STACKSIZE];

static void *thread_one_func(void *arg) {
    (void)arg;
    printf("Thread One Running\n");
    while (1) {
        /* Simulating some work */
        thread_yield();
    }
    return NULL;
}

static void *thread_two_func(void *arg) {
    (void)arg;
    printf("Thread Two Running\n");
    while (1) {
        /* Simulating some work */
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(
        thread_one_stack, sizeof(thread_one_stack), THREAD_ONE_PRIORITY,
        THREAD_CREATE_STACKTEST, thread_one_func, NULL, "thread_one");
    
    kernel_pid_t thread_two_pid = thread_create(
        thread_two_stack, sizeof(thread_two_stack), THREAD_TWO_PRIORITY,
        THREAD_CREATE_STACKTEST, thread_two_func, NULL, "thread_two");

    printf("Threads started, running at different priorities\n");

    /* Change priority at runtime */
    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 1);

    printf("Thread priorities changed\n");

    /* Main thread does nothing, just yields */
    while (1) {
        thread_yield();
    }

    return 0;
}