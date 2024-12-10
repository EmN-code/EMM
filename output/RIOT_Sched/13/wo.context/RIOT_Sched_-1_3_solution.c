#include "thread_arch.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define MY_THREAD_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define MY_THREAD_PRIO_2        (THREAD_PRIORITY_MAIN - 2)

char stack[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        /* thread code here */
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        /* thread code here */
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    kernel_pid_t thread_pid2;
    
    /* Create two threads */
    thread_pid = thread_create(stack, sizeof(stack), MY_THREAD_PRIO,
                              THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");
    thread_pid2 = thread_create(stack2, sizeof(stack2), MY_THREAD_PRIO_2,
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    /* Change priority of the second thread at runtime */
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN);

    return 0;
}